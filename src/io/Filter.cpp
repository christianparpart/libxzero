#include <xzero/io/Filter.h>
#include <xzero/io/FileRef.h>
#include <xzero/Buffer.h>
#include <xzero/sysconfig.h>
#include <stdexcept>
#include <system_error>

namespace xzero {

void Filter::applyFilters(
    const std::list<std::shared_ptr<Filter>>& filters,
    const BufferRef& input, Buffer* output, bool last) {
  auto i = filters.begin();
  auto e = filters.end();

  if (i == e) {
    *output = input;
    return;
  }

  (*i)->filter(input, output, last);
  i++;
  Buffer tmp;
  while (i != e) {
    tmp.swap(*output);
    (*i)->filter(tmp.ref(), output, last);
    i++;
  }
}

void Filter::applyFilters(
    const std::list<std::shared_ptr<Filter>>& filters,
    const FileRef& file, Buffer* output, bool last) {

  Buffer input;
  file.fill(&input);

  if (input.size() != file.size())
    throw std::runtime_error("Could not read full input file.");

  Filter::applyFilters(filters, input, output, last);
}

} // namespace xzero