#include <userver/server/handlers/http_handler_static.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/dynamic_config/value.hpp>

USERVER_NAMESPACE_BEGIN

namespace server::handlers {

namespace {

auto ParseContentTypeMap(const dynamic_config::DocsMap& docs_map) {
  return docs_map.Get("USERVER_FILES_CONTENT_TYPE_MAP")
      .As<dynamic_config::ValueDict<std::string>>();
}
constexpr dynamic_config::Key<ParseContentTypeMap> kContentTypeMap{};

}  // namespace

HttpHandlerStatic::HttpHandlerStatic(
    const components::ComponentConfig& config,
    const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      config_(context.FindComponent<components::DynamicConfig>().GetSource()),
      storage_(context
                   .FindComponent<components::FsCache>(
                       config["fs-cache-component"].As<std::string>(
                           "fs-cache-component"))
                   .GetClient()) {}

std::string HttpHandlerStatic::HandleRequestThrow(
    const http::HttpRequest& request, request::RequestContext&) const {
  LOG_DEBUG() << "Handler: " << request.GetRequestPath();
  const auto file = storage_.TryGetFile(request.GetRequestPath());
  if (file) {
    const auto config = config_.GetSnapshot();
    request.GetHttpResponse().SetContentType(
        config[kContentTypeMap][file->extension]);
    return file->data;
  }
  request.GetResponse().SetStatusNotFound();
  return "File not found";
}

}  // namespace server::handlers

USERVER_NAMESPACE_END
