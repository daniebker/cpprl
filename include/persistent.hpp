#ifndef PERSISTENT_H_
#define PERSISTENT_H_

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

class Persistent {
public :
 virtual void save(cereal::JSONOutputArchive& archive) = 0;
 virtual void load(cereal::JSONInputArchive& archive) = 0;
};

#endif // PERSISTENT_H_
