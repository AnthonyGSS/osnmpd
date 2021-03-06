/*
 * This file is part of the osnmpd project (https://github.com/verrio/osnmpd).
 * Copyright (C) 2016 Olivier Verriest
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef AGENT_AGENT_CTL_H_
#define AGENT_AGENT_CTL_H_

#include <poll.h>

/**
 * @internal
 * init_ctl_handler - initialize the control channel.
 *
 * @param poll_descriptor IN/OUT - poll file descriptor on which to register
 * for incoming events
 * @return 0 on success or -1 on any error
 */
int init_ctl_handler(struct pollfd *poll_descriptor);

/**
 * @internal
 * finish_ctl_handler - finish the control channel.
 *
 * @return 0 on success or -1 on any error
 */
int finish_ctl_handler(void);

/**
 * @internal
 * handle_ctl_request - handle an incoming control request.
 */
void handle_ctl_request(void);

#endif /* AGENT_AGENT_CTL_H_ */
