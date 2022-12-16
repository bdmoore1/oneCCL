/*
 Copyright 2016-2020 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once

#include "atl/atl_base_comm.hpp"

#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <sys/socket.h>
#include <sys/un.h>

namespace ccl {
namespace utils {
bool allgather(std::shared_ptr<atl_base_comm> comm,
               const void* send_buf,
               void* recv_buf,
               int bytes,
               bool sync = true);
bool allgatherv(std::shared_ptr<atl_base_comm> comm,
                const void* send_buf,
                void* recv_buf,
                const std::vector<int>& recv_bytes,
                bool sync = true);

int check_msg_retval(std::string operation_name,
                     ssize_t bytes,
                     struct iovec iov,
                     struct msghdr msg,
                     size_t union_size,
                     int sock,
                     int fd);
void sendmsg_fd(int sock, int fd, void* payload, int payload_len);
void recvmsg_fd(int sock, int* fd, void* payload, int payload_len);

void sendmsg_call(int sock, int fd, void* payload, int payload_len, const int rank);
void recvmsg_call(int sock, int* fd, void* payload, int payload_len, const int rank);

} // namespace utils
} // namespace ccl
