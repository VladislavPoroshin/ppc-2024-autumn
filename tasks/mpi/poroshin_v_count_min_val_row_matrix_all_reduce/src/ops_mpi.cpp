// Copyright 2023 Nesterov Alexander
#include "mpi/poroshin_v_count_min_val_row_matrix_all_reduce/include/ops_mpi.hpp"

#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

template <typename T>
void poroshin_v_count_min_val_row_matrix_all_reduce_mpi::MyTestMPITaskParallel::my_all_reduce(
    const boost::mpi::communicator& comm, const T& value, T& out_value) {
  unsigned int rank = comm.rank();
  unsigned int size = comm.size();
  unsigned int id_child_1 = 2 * rank + 1;
  unsigned int id_child_2 = 2 * rank + 2;
  unsigned int id_parent = (rank - 1) >> 1;
  out_value = value;
  T* child_1{};
  T* child_2{};
  if (id_child_1 < size) {
    child_1 = new T;
    comm.recv(id_child_1, 0, *child_1);
  }
  if (id_child_2 < size) {
    child_2 = new T;
    comm.recv(id_child_2, 0, *child_2);
  }

  if (child_1 != nullptr) {
    out_value = std::min(out_value, *child_1);
    delete child_1;
  }
  if (child_2 != nullptr) {
    out_value = std::min(out_value, *child_2);
    delete child_2;
  }

  if (rank != 0) {
    comm.send(id_parent, 0, out_value);
    comm.recv(id_parent, 0, out_value);
  }

  if (id_child_1 < size) {
    comm.send(id_child_1, 0, out_value);
  }
  if (id_child_2 < size) {
    comm.send(id_child_2, 0, out_value);
  }
}

std::vector<int> poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskSequential::gen(int m, int n) {
  std::vector<int> tmp(m * n);
  int n1 = std::max(n, m);
  int m1 = std::min(n, m);

  for (auto& t : tmp) {
    t = n1 + (std::rand() % (m1 - n1 + 7));
  }

  for (int i = 0; i < m; i++) {
    tmp[(std::rand() % n) + i * n] = INT_MIN;  // in 1 of n columns the value must be INT_MIN (needed to check answer)
  }

  return tmp;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskSequential::pre_processing() {
  internal_order_test();

  int m = taskData->inputs_count[0];
  int n = taskData->inputs_count[1];
  int size = m * n;
  input_.resize(size);
  res.resize(m);

  for (int i = 0; i < size; i++) {
    input_[i] = (reinterpret_cast<int*>(taskData->inputs[0])[i]);
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskSequential::validation() {
  internal_order_test();

  return ((!taskData->inputs.empty() && !taskData->outputs.empty()) &&
          (taskData->inputs_count.size() >= 2 && taskData->inputs_count[0] != 0 && taskData->inputs_count[1] != 0) &&
          (taskData->outputs_count[0] == taskData->inputs_count[0]));
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskSequential::run() {
  internal_order_test();

  int m = taskData->inputs_count[0];
  int n = taskData->inputs_count[1];
  int res_ = *std::min_element(input_.begin(), input_.end());

  for (int i = 0; i < m * n; i += n) {
    res[i / n] = std::count(input_.begin() + i, input_.begin() + i + n, res_);
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskSequential::post_processing() {
  internal_order_test();

  for (size_t i = 0; i < res.size(); i++) {
    reinterpret_cast<int*>(taskData->outputs[0])[i] = res[i];
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::MyTestMPITaskParallel::pre_processing() {
  internal_order_test();

  int n = 0;
  int m = 0;
  int size = 0;
  unsigned int delta = 0;

  if (world.rank() == 0) {
    m = taskData->inputs_count[0];
    n = taskData->inputs_count[1];
    size = n * m;
    if (size % world.size() == 0) {
      delta = size / world.size();
    } else {
      delta = size / world.size() + 1;
    }
    input_ = std::vector<int>(delta * world.size(), INT_MAX);
    for (int i = 0; i < size; i++) {
      input_[i] = reinterpret_cast<int*>(taskData->inputs[0])[i];
    }
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::MyTestMPITaskParallel::validation() {
  internal_order_test();

  if (world.rank() == 0) {
    return ((!taskData->inputs.empty() && !taskData->outputs.empty()) &&
            (taskData->inputs_count.size() >= 2 && taskData->inputs_count[0] != 0 && taskData->inputs_count[1] != 0) &&
            (taskData->outputs_count[0] == taskData->inputs_count[0]));
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::MyTestMPITaskParallel::run() {
  internal_order_test();

  // part 1 - finding minimums in row and matrix

  int m = 0;
  int n = 0;
  int size = 0;
  unsigned int delta = 0;

  if (world.rank() == 0) {
    m = taskData->inputs_count[0];
    n = taskData->inputs_count[1];
    size = n * m;
    if (size % world.size() == 0) {
      delta = size / world.size();
    } else {
      delta = size / world.size() + 1;
    }
  }

  broadcast(world, m, 0);
  broadcast(world, n, 0);
  broadcast(world, delta, 0);

  local_input_ = std::vector<int>(delta);
  boost::mpi::scatter(world, input_.data(), local_input_.data(), delta, 0);
  res.resize(m, INT_MAX);
  unsigned int last = 0;

  if (world.rank() == world.size() - 1) {
    last = local_input_.size() * world.size() - n * m;
  }

  unsigned int id = world.rank() * local_input_.size() / n;

  for (unsigned int i = 0; i < id; i++) {
    my_all_reduce(world, INT_MAX, res_);
  }

  delta = std::min(local_input_.size(), n - world.rank() * local_input_.size() % n);
  int l_res = *std::min_element(local_input_.begin(), local_input_.begin() + delta);
  my_all_reduce(world, l_res, res_);
  id++;
  unsigned int k = 0;

  while (local_input_.begin() + delta + k * n < local_input_.end() - last) {
    l_res = *std::min_element(local_input_.begin() + delta + k * n,
                              std::min(local_input_.end(), local_input_.begin() + delta + (k + 1) * n));
    my_all_reduce(world, l_res, res_);
    k++;
    id++;
  }

  for (unsigned int i = id; i < res.size(); i++) {
    my_all_reduce(world, INT_MAX, res_);
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  // part 2 - counting minimums in row

  id = world.rank() * local_input_.size() / n;

  for (unsigned int i = 0; i < id; i++) {
    reduce(world, INT_MAX, res[i], boost::mpi::minimum<int>(), 0);
  }

  l_res = std::count(local_input_.begin(), local_input_.begin() + delta, res_);
  reduce(world, l_res, res[id], boost::mpi::minimum<int>(), 0);
  id++;
  k = 0;

  while (local_input_.begin() + delta + k * n < local_input_.end() - last) {
    l_res = std::count(local_input_.begin() + delta + k * n,
                       std::min(local_input_.end(), local_input_.begin() + delta + (k + 1) * n), res_);
    reduce(world, l_res, res[id], boost::mpi::minimum<int>(), 0);
    k++;
    id++;
  }

  for (unsigned int i = id; i < res.size(); i++) {
    reduce(world, INT_MAX, res[i], boost::mpi::minimum<int>(), 0);
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::MyTestMPITaskParallel::post_processing() {
  internal_order_test();

  if (world.rank() == 0) {
    for (size_t i = 0; i < res.size(); i++) {
      reinterpret_cast<int*>(taskData->outputs[0])[i] = res[i];
    }
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskParallel::pre_processing() {
  internal_order_test();

  int n = 0;
  int m = 0;
  int size = 0;
  unsigned int delta = 0;

  if (world.rank() == 0) {
    m = taskData->inputs_count[0];
    n = taskData->inputs_count[1];
    size = n * m;
    if (size % world.size() == 0) {
      delta = size / world.size();
    } else {
      delta = size / world.size() + 1;
    }
    input_ = std::vector<int>(delta * world.size(), INT_MAX);
    for (int i = 0; i < size; i++) {
      input_[i] = reinterpret_cast<int*>(taskData->inputs[0])[i];
    }
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskParallel::validation() {
  internal_order_test();

  if (world.rank() == 0) {
    return ((!taskData->inputs.empty() && !taskData->outputs.empty()) &&
            (taskData->inputs_count.size() >= 2 && taskData->inputs_count[0] != 0 && taskData->inputs_count[1] != 0) &&
            (taskData->outputs_count[0] == taskData->inputs_count[0]));
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskParallel::run() {
  internal_order_test();

  // part 1 - finding minimums in row and matrix

  int m = 0;
  int n = 0;
  int size = 0;
  unsigned int delta = 0;

  if (world.rank() == 0) {
    m = taskData->inputs_count[0];
    n = taskData->inputs_count[1];
    size = n * m;
    if (size % world.size() == 0) {
      delta = size / world.size();
    } else {
      delta = size / world.size() + 1;
    }
  }

  broadcast(world, m, 0);
  broadcast(world, n, 0);
  broadcast(world, delta, 0);

  local_input_ = std::vector<int>(delta);
  boost::mpi::scatter(world, input_.data(), local_input_.data(), delta, 0);
  res.resize(m, INT_MAX);
  unsigned int last = 0;

  if (world.rank() == world.size() - 1) {
    last = local_input_.size() * world.size() - n * m;
  }

  unsigned int id = world.rank() * local_input_.size() / n;

  for (unsigned int i = 0; i < id; i++) {
    all_reduce(world, INT_MAX, res_, boost::mpi::minimum<int>());
  }

  delta = std::min(local_input_.size(), n - world.rank() * local_input_.size() % n);
  int l_res = *std::min_element(local_input_.begin(), local_input_.begin() + delta);
  all_reduce(world, l_res, res_, boost::mpi::minimum<int>());
  id++;
  unsigned int k = 0;

  while (local_input_.begin() + delta + k * n < local_input_.end() - last) {
    l_res = *std::min_element(local_input_.begin() + delta + k * n,
                              std::min(local_input_.end(), local_input_.begin() + delta + (k + 1) * n));
    all_reduce(world, l_res, res_, boost::mpi::minimum<int>());
    k++;
    id++;
  }

  for (unsigned int i = id; i < res.size(); i++) {
    all_reduce(world, INT_MAX, res_, boost::mpi::minimum<int>());
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  // part 2 - counting minimums in row

  id = world.rank() * local_input_.size() / n;

  for (unsigned int i = 0; i < id; i++) {
    reduce(world, INT_MAX, res[i], boost::mpi::minimum<int>(), 0);
  }

  l_res = std::count(local_input_.begin(), local_input_.begin() + delta, res_);
  reduce(world, l_res, res[id], boost::mpi::minimum<int>(), 0);
  id++;
  k = 0;

  while (local_input_.begin() + delta + k * n < local_input_.end() - last) {
    l_res = std::count(local_input_.begin() + delta + k * n,
                       std::min(local_input_.end(), local_input_.begin() + delta + (k + 1) * n), res_);
    reduce(world, l_res, res[id], boost::mpi::minimum<int>(), 0);
    k++;
    id++;
  }

  for (unsigned int i = id; i < res.size(); i++) {
    reduce(world, INT_MAX, res[i], boost::mpi::minimum<int>(), 0);
  }

  return true;
}

bool poroshin_v_count_min_val_row_matrix_all_reduce_mpi::TestMPITaskParallel::post_processing() {
  internal_order_test();

  if (world.rank() == 0) {
    for (size_t i = 0; i < res.size(); i++) {
      reinterpret_cast<int*>(taskData->outputs[0])[i] = res[i];
    }
  }

  return true;
}