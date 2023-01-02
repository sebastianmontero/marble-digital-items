cmake_minimum_required(VERSION 3.5)                                                                                                                                    

project(marble)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(cdt)

add_contract( marble marble 
                marble.cpp
                setting.cpp
            )
                
target_include_directories( marble PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( marble ${CMAKE_SOURCE_DIR}/../resources )