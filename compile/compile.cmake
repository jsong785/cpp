add_library(compile INTERFACE)
target_compile_features(compile INTERFACE cxx_std_20)
target_compile_options(compile INTERFACE -O3)
