// Copyright 2023 Nesterov Alexander
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/include/ops_seq.hpp
namespace poroshin_v_find_min_val_row_matrix_seq {
========
namespace frolova_e_num_of_letters_seq {

int Count(std::string& str);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/frolova_e_num_of_letters/include/ops_seq.hpp

class TestTaskSequential : public ppc::core::Task {
 public:
  explicit TestTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  static std::vector<int> gen(int m, int n);  // generate vector (matrix)

 private:
  std::vector<int> input_{}, res{};
  // notation for TaskData
  // inputs - vector (matrix)
  // inputs_count[0] - m, inputs_count[1] - n
  //  m - num of rows, n - num of columns
};

<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/include/ops_seq.hpp
}  // namespace poroshin_v_find_min_val_row_matrix_seq
========
}  // namespace frolova_e_num_of_letters_seq
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/frolova_e_num_of_letters/include/ops_seq.hpp
