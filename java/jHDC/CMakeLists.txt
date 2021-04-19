find_program(MVN_EXECUTABLE mvn)
# we could use REQUIRED instead, but cmake 3.18 is imho quite new:
if (NOT MVN_EXECUTABLE)
    message(ERROR "\"mvn\" command not found")
endif()

if (INSTALL_JAVA_DEPENDENCIES)
    set(MVN_ARGS "dependency:copy-dependencies")
endif()

if (NOT JAR_WITH_DEPENDENCIES)
    # just make jar without dependencies
    set(JHDC_JAR_NAME "${CMAKE_CURRENT_BINARY_DIR}/target/jHDC-${CMAKE_PROJECT_VERSION}.jar")
    add_custom_command(OUTPUT ${JHDC_JAR_NAME}
        COMMAND ${MVN_EXECUTABLE} ${MVN_ARGS} package
    )
    add_custom_target(jar ALL DEPENDS ${JHDC_JAR_NAME})
    add_dependencies(jar hdc-jni)
    # install the resulting jar
    install(FILES ${JHDC_JAR_NAME} DESTINATION ${JAR_INSTALL_PREFIX})

    if (INSTALL_JAVA_DEPENDENCIES)
        # install dependencies
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/target/dependency/ DESTINATION ${JAR_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.jar")
    endif() # INSTALL_JAVA_DEPENDENCIES

else()
    if (INSTALL_JAVA_DEPENDENCIES)
        message("Ignoring INSTALL_JAVA_DEPENDENCIES since they are not needed with bundled jar.")
    endif()
    # create überjar with everything included
    set(JHDC_JAR_NAME "${CMAKE_CURRENT_BINARY_DIR}/target/jHDC-${CMAKE_PROJECT_VERSION}-jar-with-dependencies.jar")
    add_custom_command(OUTPUT ${JHDC_JAR_NAME}
        COMMAND ${MVN_EXECUTABLE} compile assembly:single -Djavacpp.platform=linux-x86_64
    )
    add_custom_target(jar-with-dependencies ALL DEPENDS ${JHDC_JAR_NAME})
    add_dependencies(jar-with-dependencies hdc-jni)
    # install the resulting jar
    install(FILES ${JHDC_JAR_NAME} DESTINATION ${JAR_INSTALL_PREFIX})
endif() # JAR_WITH_DEPENDENCIES