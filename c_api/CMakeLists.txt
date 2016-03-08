# *************************************************************************/
# * Copyright (c) 2015 Ruslan Baratov.                                    */
# *                                                                       */
# * Permission is hereby granted, free of charge, to any person obtaining */
# * a copy of this software and associated documentation files (the       */
# * "Software"), to deal in the Software without restriction, including   */
# * without limitation the rights to use, copy, modify, merge, publish,   */
# * distribute, sublicense, and/or sell copies of the Software, and to    */
# * permit persons to whom the Software is furnished to do so, subject to */
# * the following conditions:                                             */
# *                                                                       */
# * The above copyright notice and this permission notice shall be        */
# * included in all copies or substantial portions of the Software.       */
# *                                                                       */
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
# * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
# * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
# * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
# * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
# * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
# * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
# *************************************************************************/

cmake_minimum_required(VERSION 3.0)
project(CSpdlogAPi)


if (CMAKE_COMPILER_IS_GNUCXX)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -pthread -O3 ")  
  set ( CMAKE_EXE_LINKER_FLAGS "-pthread")  
endif ()

#add_executable(example example.cpp)
add_library(cspdlog_obj OBJECT cspdlog.cpp)

set_property(TARGET cspdlog_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(cspdlog_shared SHARED $<TARGET_OBJECTS:cspdlog_obj>)
add_library(cspdlog_static STATIC $<TARGET_OBJECTS:cspdlog_obj>)
set_target_properties(cspdlog_shared PROPERTIES OUTPUT_NAME cspdlog CLEAN_DIRECT_OUTPUT 1)
set_target_properties(cspdlog_static PROPERTIES OUTPUT_NAME cspdlog CLEAN_DIRECT_OUTPUT 1)

#target_link_libraries(example )
include_directories("../include")
target_link_libraries(cspdlog_shared stdc++)
target_link_libraries(cspdlog_static stdc++)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")
install(TARGETS cspdlog_shared cspdlog_static 
		LIBRARY DESTINATION "lib"
		ARCHIVE DESTINATION "lib"
)
install(FILES cspdlog.h DESTINATION "include")

