cmake_minimum_required(VERSION 3.16)

project(IPStack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_FILE_PATH "${CMAKE_SOURCE_DIR}/models/schema.json")
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/models")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Quick Qml QuickWidgets Test Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Quick Qml Positioning QuickWidgets Test Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IPStack
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        db/IDataStorage.h
        models/Geolocation.h
        api_client/IpStackApiClient.h api_client/IpStackApiClient.cpp


        models/ApiResponse.h
        rapidjson/error/en.h rapidjson/error/error.h rapidjson/internal/biginteger.h rapidjson/internal/clzll.h rapidjson/internal/diyfp.h rapidjson/internal/dtoa.h rapidjson/internal/ieee754.h rapidjson/internal/itoa.h rapidjson/internal/meta.h rapidjson/internal/pow10.h rapidjson/internal/regex.h rapidjson/internal/stack.h rapidjson/internal/strfunc.h rapidjson/internal/strtod.h rapidjson/internal/swap.h rapidjson/msinttypes/inttypes.h rapidjson/msinttypes/stdint.h rapidjson/allocators.h rapidjson/cursorstreamwrapper.h rapidjson/document.h rapidjson/encodedstream.h rapidjson/encodings.h rapidjson/filereadstream.h rapidjson/filewritestream.h rapidjson/fwd.h rapidjson/istreamwrapper.h rapidjson/memorybuffer.h rapidjson/memorystream.h rapidjson/ostreamwrapper.h rapidjson/pointer.h rapidjson/prettywriter.h rapidjson/rapidjson.h rapidjson/reader.h rapidjson/schema.h rapidjson/stream.h rapidjson/stringbuffer.h rapidjson/uri.h rapidjson/writer.h
        api_client/IGeolocationProvider.h
        api_client/IpStackResponseParser.h api_client/IpStackResponseParser.cpp

        Maps.qrc
        models/ItemListModel.h
        db/SqlLiteDataStorage.h db/SqlLiteDataStorage.cpp
        api_client/IIPResolver.h
        api_client/IpResolver.h api_client/IpResolver.cpp
    )

enable_testing()

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET IPStack APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(IPStack SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(IPStack
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(IPStack PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt6::QuickWidgets
    Qt6::Test
    Qt6::Sql
    ws2_32)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.IPStack)
endif()
set_target_properties(IPStack PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


include(GNUInstallDirs)
install(TARGETS IPStack
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
    TARGET IPStack PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${JSON_FILE_PATH} ${OUTPUT_DIR}/schema.json
    COMMENT "Copying JSON file to output directory"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(IPStack)
endif()
add_subdirectory(Test)
