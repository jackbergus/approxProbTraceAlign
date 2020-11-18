//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/LogBenchmark.h"

void LogBenchmark::log_record_element_per_query(std::ostream &file, const std::vector<record_element_per_query> &log) {
    file << "input_file,fraces_file,query,query_length,embeddingStrategy,trace_noise,dulcior,tuning_factor,use_path_with_lambda_factor,lambda,max_length,min_prob,rankingDistance,spearman,proposedMetric" << std::endl;
    for (const record_element_per_query& record : log ) {
        file <<  std::boolalpha
             << record.input_file <<','
             << record.traces_file << ','
             << record.query << ','
             << record.query_length<< ','
             << magic_enum::enum_name<PathEmbeddingStrategy>(record.embeddingStrategy).data() << ','
             << record.trace_noise << ','
             << record.dulcior << ','
             << record.tuning_factor <<','
             << record.use_path_with_lambda_factor << ','
             << record.lambda << ','
             << record.max_length << ','
             << record.min_prob << ','
             << record.rankingDistance << ','
             << record.spearman << ','
             << record.proposedMetric << std::endl;
    }
}

void LogBenchmark::log_record_element_per_log(std::ostream &file, const std::vector<additional_benchmarks_per_log> &log) {
    file << "input_file,fraces_file,query,query_length,embeddingStrategy,trace_noise,dulcior,tuning_factor,use_path_with_lambda_factor,lambda,max_length,min_prob,metric_type,value,logtrace,logtrace_size" << std::endl;
    for (const additional_benchmarks_per_log& record : log ) {
        file <<  std::boolalpha
             << record.input_file <<','
             << record.traces_file << ','
             << record.query << ','
             << record.query_length<< ','
             << magic_enum::enum_name<PathEmbeddingStrategy>(record.embeddingStrategy).data() << ','
             << record.trace_noise << ','
             << record.dulcior << ','
             << record.tuning_factor <<','
             << record.use_path_with_lambda_factor << ','
             << record.lambda << ','
             << record.max_length << ','
             << record.min_prob << ','
             << magic_enum::enum_name<LogDependantQualityMetricTypes>(record.metricType).data() << ','
             << record.value << ','
             << record.logtrace << ','
             << record.logtrace_size << std::endl;
    }
}

void LogBenchmark::log_record_element_per_query(std::ostream &file) {
    file << "input_file,fraces_file,query,query_length,embeddingStrategy,trace_noise,dulcior,tuning_factor,use_path_with_lambda_factor,lambda,max_length,min_prob,rankingDistance,spearman,proposedMetric" << std::endl;
}

void LogBenchmark::log_record_element_per_log(std::ostream &file) {
    file << "input_file,fraces_file,query,query_length,embeddingStrategy,trace_noise,dulcior,tuning_factor,use_path_with_lambda_factor,lambda,max_length,min_prob,metric_type,value,logtrace,logtrace_size" << std::endl;
}
