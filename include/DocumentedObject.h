#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H
#include <sstream>
#include <string>

namespace StreamFlow
{

class DocumentedObject
{
public:
  std::string name() const noexcept { return objectName; }
  std::string getObjectDocString() const noexcept { return docString; }
  virtual std::string doc() const
  {
    std::ostringstream oss;
    oss << "name=" << objectName << ", desc=" << docString << " ";
    return oss.str();
  }
  virtual ~DocumentedObject() = default;

  void setName(std::string aName) noexcept { objectName = aName; }
  void setDescription(std::string aDesc) noexcept { docString = aDesc; }

private:
  std::string objectName = "no_name";
  std::string docString = "no_desc";
};
}
#endif // OBJECT_BASE_H
