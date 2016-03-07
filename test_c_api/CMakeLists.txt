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
project(CSpdlogAPITests)

set(CMAKE_C_FLAGS "-Wall -O3 -pthread")

set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_C_FLAGS}")  

add_executable(tests testcspdlog.c)
add_executable(cspdlog-bench spdlog-bench.c)
add_executable(example example.c)
include_directories( "./c_api" )
add_subdirectory( "./c_api" )
#find_library(CSPDLOG NAMES cspdlog PATH "c_api/build" )

target_link_libraries(tests  cspdlog)
target_link_libraries(cspdlog-bench  cspdlog)
target_link_libraries(example  cspdlog)
#file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")
