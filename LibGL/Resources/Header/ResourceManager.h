#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

class IResource 
{
public:
    virtual ~IResource() = default;
};

template <typename T>
class ResourceHolder : public IResource 
{
public:
    std::unique_ptr<T> resource;

    ResourceHolder(std::unique_ptr<T> res) : resource(std::move(res)) {}

    T* get() 
    {
        return resource.get();
    }
};

class ResourceManager 
{
private:
    std::unordered_map<std::string, std::unique_ptr<IResource>> resources;

public:
    template <typename T>
    T* Create(const std::string& name) 
    {
        std::unique_ptr<T> newResource = std::make_unique<T>();

        if (!newResource->load(name)) 
        {
            std::cerr << "Failed to create resource: " << name << std::endl;

            return nullptr;
        }

        resources[name] = std::make_unique<ResourceHolder<T>>(std::move(newResource));

        return Get<T>(name);
    }

    template <typename T>
    T* Get(const std::string& name) 
    {
        auto it = resources.find(name);

        if (it != resources.end()) 
        {
            auto holder = dynamic_cast<ResourceHolder<T>*>(it->second.get());

            if (holder) 
            {
                return holder->get();
            }

            else 
            {
                std::cerr << "Type mismatch for resource: " << name << std::endl;
            }
        }

        return nullptr;
    }

    void Delete(const std::string& name) 
    {
        resources.erase(name);
    }

    void clear()
    {
        resources.clear();
    }
};
