ENABLE_QT()

file(GLOB HDRS *.h)
file(GLOB SRCS *.cpp *.c)
set (SRCS ${HDRS} ${SRCS})

# TODO: put QT Moc files here.
#set (MOC_HDRS game_widget.h mwo_application.h mwo_mainwindow.h)
#QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

# TODO: put resources here.
#set (RESOURCE_SRCS images.qrc)
#QT4_ADD_RESOURCES(IMAGES_RES ${RESOURCE_SRCS})

# TODO: put output file name here.
add_executable(onyx-test.oar ${SRCS} ${MOC_SRCS} ${IMAGES_RES})
target_link_libraries(onyx-test.oar
   onyx_data
   onyx_screen
   onyx_sys
   onyx_ui
   ${QT_LIBRARIES}
   ${ADD_LIB}
)

