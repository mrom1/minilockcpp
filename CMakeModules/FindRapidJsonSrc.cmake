include(ExternalProject)

SET(RAPIDJSON_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/rapidjson")

ExternalProject_Add(rapidjson
  GIT_REPOSITORY https://github.com/miloyip/rapidjson.git
  #GIT_TAG stable
  PREFIX ${RAPIDJSON_SEARCH_PATH}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)


ExternalProject_Get_Property(rapidjson source_dir)


set(RAPIDJSON_INCLUDES ${source_dir}/include)

#include_directories(${source_dir}/include)

