#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H
#include <string>
#include <sstream>

namespace StreamFlow {



class ObjectBase
{
  public:
    std::string name() const noexcept {return objectName;}
    std::string description() const noexcept {return objectDescription;}
    virtual std::string describe() const
    {
        std::ostringstream oss;
        oss << "name="<< objectName << ", desc=" << objectDescription << " "  ;
        return oss.str();
    };

    void setName(std::string aName) noexcept {objectName = aName;}
    void setDescription(std::string aDesc) noexcept {objectDescription = aDesc;}
  private:
    std::string objectName="no_name";
    std::string objectDescription="no_desc";
};

}
#endif // OBJECT_BASE_H
