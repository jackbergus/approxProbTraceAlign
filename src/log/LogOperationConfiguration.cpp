//
// Created by giacomo on 13/11/20.
//

#include <log/LogOperationConfiguration.h>

LogOperationConfiguration::LogOperationConfiguration() {}

LogOperationConfiguration::LogOperationConfiguration(LogOperations operation, double factor, bool keepLowUpOtherwise) : operation(operation),
                                                                                                                        factor(factor),
                                                                                                                        keep_low_up_otherwise(
                                                                                                                                keepLowUpOtherwise) {}
