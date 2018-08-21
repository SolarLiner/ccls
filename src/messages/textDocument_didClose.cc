// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "clang_complete.h"
#include "message_handler.h"
#include "pipeline.hh"
#include "working_files.h"
using namespace ccls;

namespace {
MethodType kMethodType = "textDocument/didClose";

struct In_TextDocumentDidClose : public NotificationInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  struct Params {
    lsTextDocumentIdentifier textDocument;
  };
  Params params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentDidClose::Params, textDocument);
MAKE_REFLECT_STRUCT(In_TextDocumentDidClose, params);
REGISTER_IN_MESSAGE(In_TextDocumentDidClose);

struct Handler_TextDocumentDidClose
    : BaseMessageHandler<In_TextDocumentDidClose> {
  MethodType GetMethodType() const override { return kMethodType; }

  void Run(In_TextDocumentDidClose *request) override {
    std::string path = request->params.textDocument.uri.GetPath();

    // Clear any diagnostics for the file.
    Out_TextDocumentPublishDiagnostics out;
    out.params.uri = request->params.textDocument.uri;
    pipeline::WriteStdout(kMethodType, out);

    // Remove internal state.
    working_files->OnClose(request->params.textDocument);
    clang_complete->NotifyClose(path);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentDidClose);
} // namespace
