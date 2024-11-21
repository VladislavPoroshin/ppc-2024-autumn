// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
#include "seq/poroshin_v_find_min_val_row_matrix/include/ops_seq.hpp"

TEST(poroshin_v_find_min_val_row_matrix_seq, test_pipeline_run) {
  // Create data
  const int n = 5000;
  const int m = 5000;

========
#include "seq/grudzin_k_nearest_neighbor_elements/include/ops_seq.hpp"

TEST(grudzin_k_nearest_neighbor_elements_seq, test_pipeline_run) {
  int size = 10000000;
  // Create data
  std::vector<int> in(size);
  std::vector<int> out(1, 0);
  int ans = 0;
  for (int i = 0; i < size; ++i) {
    in[i] = 3 * i;
  }
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/grudzin_k_nearest_neighbor_elements/perf_tests/main.cpp
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

  // Create Task
<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
  auto testTaskSequential = std::make_shared<poroshin_v_find_min_val_row_matrix_seq::TestTaskSequential>(taskDataSeq);

  std::vector<int> tmp = poroshin_v_find_min_val_row_matrix_seq::TestTaskSequential::gen(m, n);

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(m);
  taskDataSeq->inputs_count.emplace_back(n);
  std::vector<int> result(m);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(m);
========
  auto testTaskSequential = std::make_shared<grudzin_k_nearest_neighbor_elements_seq::TestTaskSequential>(taskDataSeq);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/grudzin_k_nearest_neighbor_elements/perf_tests/main.cpp

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;  // Set the number of runs as needed
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp

  for (int i = 0; i < m; i++) {
    ASSERT_EQ(result[i], INT_MIN);
  }
}

TEST(poroshin_v_find_min_val_row_matrix_seq, test_task_run) {
  // Create data
  const int n = 5000;
  const int m = 5000;

========
  ASSERT_EQ(ans, out[0]);
}

TEST(grudzin_k_nearest_neighbor_elements_seq, test_task_run) {
  int size = 10000000;
  // Create data
  std::vector<int> in(size);
  std::vector<int> out(1, 0);
  int ans = 0;
  for (int i = 0; i < size; ++i) {
    in[i] = 2 * i;
  }
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/grudzin_k_nearest_neighbor_elements/perf_tests/main.cpp
  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

  // Create Task
<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp
  auto testTaskSequential = std::make_shared<poroshin_v_find_min_val_row_matrix_seq::TestTaskSequential>(taskDataSeq);

  std::vector<int> tmp = poroshin_v_find_min_val_row_matrix_seq::TestTaskSequential::gen(m, n);

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(tmp.data()));
  taskDataSeq->inputs_count.emplace_back(m);
  taskDataSeq->inputs_count.emplace_back(n);
  std::vector<int> result(m);
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(result.data()));
  taskDataSeq->outputs_count.emplace_back(m);
========
  auto testTaskSequential = std::make_shared<grudzin_k_nearest_neighbor_elements_seq::TestTaskSequential>(taskDataSeq);
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/grudzin_k_nearest_neighbor_elements/perf_tests/main.cpp

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
<<<<<<<< HEAD:tasks/seq/poroshin_v_find_min_val_row_matrix/perf_tests/main.cpp

  for (int i = 0; i < m; i++) {
    ASSERT_EQ(result[i], INT_MIN);
  }
}
========
  ASSERT_EQ(ans, out[0]);
}
>>>>>>>> 25edc662edec4c78f3f0e08872048495e6a58497:tasks/seq/grudzin_k_nearest_neighbor_elements/perf_tests/main.cpp
