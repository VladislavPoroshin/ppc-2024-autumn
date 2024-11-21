// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <boost/mpi/timer.hpp>
#include <vector>

#include "core/perf/include/perf.hpp"
<<<<<<<< HEAD:tasks/mpi/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
#include "mpi/poroshin_v_find_min_val_row_matrix/include/ops_mpi.hpp"

TEST(poroshin_v_find_min_val_row_matrix_mpi, test_pipeline_run) {
  int m = 1000;
  int n = 4000;
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  std::vector<int32_t> m_vec(m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();

  if (world.rank() == 0) {
    global_matrix = std::vector<int>(m * n, 1);
    taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_matrix.data()));
    taskDataPar->inputs_count.emplace_back(m);
    taskDataPar->inputs_count.emplace_back(n);
    taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(m_vec.data()));
    taskDataPar->outputs_count.emplace_back(m_vec.size());
  }

  auto testMpiTaskParallel = std::make_shared<poroshin_v_find_min_val_row_matrix_mpi::TestMPITaskParallel>(taskDataPar);
========
#include "mpi/frolova_e_num_of_letters/include/ops_mpi.hpp"

std::string GenStr(int n) {
  if (n <= 0) {
    return std::string();
  }
  std::string str = "test";
  std::string result;
  result.resize(n);

  int i = 0;
  size_t j = 0;

  while (i < n) {
    result[i] = str[j];
    j++;
    i++;
    if (j >= str.size()) {
      j = 0;
    }
  }
  return result;
}

TEST(frolova_e_num_of_letters, test_pipeline_run) {
  boost::mpi::communicator world;
  std::string global_str;
  std::vector<int32_t> global_sum(1, 0);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  int count_size_;
  if (world.rank() == 0) {
    count_size_ = 120;
    global_str = GenStr(count_size_);
    taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_str.data()));
    taskDataPar->inputs_count.emplace_back(global_str.size());
    taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(global_sum.data()));
    taskDataPar->outputs_count.emplace_back(global_sum.size());
  }

  auto testMpiTaskParallel = std::make_shared<frolova_e_num_of_letters_mpi::TestMPITaskParallel>(taskDataPar);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/mpi/frolova_e_num_of_letters/perf_tests/main.cpp
  ASSERT_EQ(testMpiTaskParallel->validation(), true);
  testMpiTaskParallel->pre_processing();
  testMpiTaskParallel->run();
  testMpiTaskParallel->post_processing();

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const boost::mpi::timer current_timer;
  perfAttr->current_timer = [&] { return current_timer.elapsed(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testMpiTaskParallel);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);

  if (world.rank() == 0) {
    ppc::core::Perf::print_perf_statistic(perfResults);
<<<<<<<< HEAD:tasks/mpi/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
    for (unsigned i = 0; i < m_vec.size(); i++) {
      EXPECT_EQ(1, m_vec[i]);
    }
  }
}

TEST(poroshin_v_find_min_val_row_matrix_mpi, test_task_run) {
  int m = 1000;
  int n = 4000;
  boost::mpi::communicator world;
  std::vector<int> global_matrix;
  std::vector<int32_t> m_vec(m);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();

  if (world.rank() == 0) {
    global_matrix = std::vector<int>(m * n, 1);
    taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_matrix.data()));
    taskDataPar->inputs_count.emplace_back(m);
    taskDataPar->inputs_count.emplace_back(n);
    taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(m_vec.data()));
    taskDataPar->outputs_count.emplace_back(m_vec.size());
  }

  auto testMpiTaskParallel = std::make_shared<poroshin_v_find_min_val_row_matrix_mpi::TestMPITaskParallel>(taskDataPar);
========
    ASSERT_EQ(count_size_, global_sum[0]);
  }
}

TEST(frolova_e_num_of_letters, test_task_run) {
  boost::mpi::communicator world;
  std::string global_str;
  std::vector<int32_t> global_sum(1, 0);
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  int count_size_;
  if (world.rank() == 0) {
    count_size_ = 120;
    global_str = GenStr(count_size_);
    taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_str.data()));
    taskDataPar->inputs_count.emplace_back(global_str.size());
    taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(global_sum.data()));
    taskDataPar->outputs_count.emplace_back(global_sum.size());
  }

  auto testMpiTaskParallel = std::make_shared<frolova_e_num_of_letters_mpi::TestMPITaskParallel>(taskDataPar);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/mpi/frolova_e_num_of_letters/perf_tests/main.cpp
  ASSERT_EQ(testMpiTaskParallel->validation(), true);
  testMpiTaskParallel->pre_processing();
  testMpiTaskParallel->run();
  testMpiTaskParallel->post_processing();

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const boost::mpi::timer current_timer;
  perfAttr->current_timer = [&] { return current_timer.elapsed(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testMpiTaskParallel);
  perfAnalyzer->task_run(perfAttr, perfResults);

  if (world.rank() == 0) {
    ppc::core::Perf::print_perf_statistic(perfResults);
<<<<<<<< HEAD:tasks/mpi/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
    for (unsigned i = 0; i < m_vec.size(); i++) {
      EXPECT_EQ(1, m_vec[i]);
    }
========
    ASSERT_EQ(count_size_, global_sum[0]);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/mpi/frolova_e_num_of_letters/perf_tests/main.cpp
  }
}
