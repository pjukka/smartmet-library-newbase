find_package(Fmt)

if(NOT Boost_INCLUDE_DIRS)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    find_package(Boost 1.60.0 QUIET COMPONENTS regex thread filesystem iostreams system date_time)
    if (Boost_FOUND AND NOT BOOST_INCLUDE_DIR)
        set(BOOST_INCLUDE_DIR "${Boost_INCLUDE_DIR}")
    endif()
endif()
