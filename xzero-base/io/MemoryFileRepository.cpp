#include <xzero-base/io/MemoryFileRepository.h>
#include <xzero-base/MimeTypes.h>

namespace xzero {

MemoryFileRepository::MemoryFileRepository(MimeTypes& mimetypes)
    : mimetypes_(mimetypes),
      files_(),
      notFound_(new MemoryFile()) {
}

std::shared_ptr<File> MemoryFileRepository::getFile(
    const std::string& requestPath,
    const std::string& /*docroot*/) {
  auto i = files_.find(requestPath);
  if (i != files_.end())
    return i->second;

  return notFound_;
}

void MemoryFileRepository::insert(
    const std::string& path, const BufferRef& data, DateTime mtime) {
  files_[path].reset(new MemoryFile(path,
                                    mimetypes_.getMimeType(path),
                                    data,
                                    mtime));
}

void MemoryFileRepository::insert(
    const std::string& path, const BufferRef& data) {
  insert(path, data, DateTime());
}

} // namespace xzero
