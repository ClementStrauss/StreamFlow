#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H
#include <sstream>
#include <string>

namespace StreamFlow {

class DocumentedObject {
 public:
  virtual ~DocumentedObject() = default;

  virtual std::string name() const noexcept { return objectName; }
  void setName(std::string aName) noexcept { objectName = aName; }

  std::string docString() const noexcept { return documentationString; }
  void setDocString(std::string aDesc) noexcept { documentationString = aDesc; }

  virtual std::string doc(bool verbose = false) const {
    std::ostringstream oss;
    oss << objectName << ": " << std::endl;
    oss << "  " << documentationString << std::endl;
    return oss.str();
  }

 private:
  std::string objectName = "no_name";
  std::string documentationString = "no_desc";
};
}  // namespace StreamFlow
#endif  // OBJECT_BASE_H
