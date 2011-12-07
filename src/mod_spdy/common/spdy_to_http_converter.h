// Copyright 2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MOD_SPDY_SPDY_TO_HTTP_CONVERTER_H_
#define MOD_SPDY_SPDY_TO_HTTP_CONVERTER_H_

#include "net/spdy/spdy_framer.h"

namespace mod_spdy {

class HttpStreamVisitorInterface;

// SpdyFramerVisitorInterface that converts SpdyFrames to HTTP
// streams, and passes the HTTP stream to the specified
// HttpStreamVisitorInterface.
class SpdyToHttpConverter : public spdy::SpdyFramerVisitorInterface {
 public:
  SpdyToHttpConverter(spdy::SpdyFramer *framer,
                      HttpStreamVisitorInterface *visitor);
  virtual ~SpdyToHttpConverter();

  virtual void OnError(spdy::SpdyFramer *framer);
  virtual void OnControl(const spdy::SpdyControlFrame *frame);
  virtual void OnStreamFrameData(spdy::SpdyStreamId stream_id,
                                 const char *data,
                                 size_t len);

  bool HasError() const { return error_; }

private:
  void OnSynStream(const spdy::SpdySynStreamControlFrame *frame);

  spdy::SpdyFramer *const framer_;
  HttpStreamVisitorInterface *const visitor_;
  bool error_;
};

// Generate an HTTP request line from the given SPDY header block by calling
// the OnStatusLine() method of the given visitor, and return true.  If there's
// an error, this will return false without calling any methods on the visitor.
bool GenerateRequestLineFromHeaderBlock(const spdy::SpdyHeaderBlock& headers,
                                        HttpStreamVisitorInterface* visitor);

// Convert the given SPDY header block (e.g. from a SYN_STREAM, SYN_REPLY, or
// HEADERS frame) into HTTP headers by calling the OnHeader() method of the
// given visitor.  Note that this does not call OnStatusLine() or
// OnHeadersComplete() on the visitor.
void GenerateHeadersFromHeaderBlock(const spdy::SpdyHeaderBlock& headers,
                                    HttpStreamVisitorInterface* visitor);

}  // namespace mod_spdy

#endif  // MOD_SPDY_SPDY_TO_HTTP_CONVERTER_H_
