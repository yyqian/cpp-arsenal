aux_source_directory(. SOURCE_FILES)
add_library(arsenal_static STATIC ${SOURCE_FILES})
set_target_properties(arsenal_static PROPERTIES OUTPUT_NAME arsenal)
