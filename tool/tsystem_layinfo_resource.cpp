#include "tsystem_layinfo_resource.h"

#include <algorithm>

#include "TLib/core/tsystem_appbase.h"
#include "TLib/core/tsystem_app_registration.h"
#include "TLib/core/tsystem_core_common.h"
#include "TLib/core/tsystem_core_paths.h"

using namespace TSystem::layinfo;
using namespace TSystem;

AppResource::AppResource()
	: name("")
	, type("")
	, dev_tag("")
	, node("")
	, domain("")
	, host("")
	, port(0)
	, pid(0)
	, raid_id(0)
{};

AppResource::operator bool() const
{
	return !(name.empty() || type.empty() || dev_tag.empty() || !pid );
}

AppResource AppResourceManager::bad_resource = AppResource();

AppResourceManager::AppResourceManager()
	: pid_maps_(256)
	, resources_()
{
	resources_.reserve(256);
}

AppResourceManager::AppResourceManager(AppResourceManager&& in)
	: pid_maps_(std::move(in.pid_maps_))
	, resources_(std::move(in.resources_))
{}

AppResourceManager& AppResourceManager::operator=(AppResourceManager&& in)
{
	if( &in != this )
	{
		pid_maps_ = std::move(in.pid_maps_);
		resources_ = std::move(in.resources_);
	}
	return *this;
}

void AppResourceManager::RegisterResource(const AppResource& resource )
{
	if( !resource )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "AppResourceManager::RegisterResource"
			, "try to add empty resource");

	if( std::find_if(std::begin(resources_), std::end(resources_)
		, [&resource](const std::unique_ptr<AppResource>& p_res)->bool{ return p_res->pid == resource.pid; }) 
		!= std::end(resources_) )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "AppResourceManager::RegisterResource"
			, utility::FormatStr("resource with pid %d already registered", resource.pid));

	resources_.push_back(std::unique_ptr<AppResource>(new AppResource(resource)));
}

void AppResourceManager::MapResource()
{
	pid_maps_.clear();
	std::for_each(std::begin(resources_), std::end(resources_), [this](const std::unique_ptr<AppResource>& res)
	{ pid_maps_.insert( std::make_pair(res->pid, res.get()) ); });
}

const AppResource& AppResourceManager::FindUnique(unsigned short pid) const
{
	auto it = pid_maps_.find(pid);
	return it != pid_maps_.end() ? *it->second : bad_resource;
}

const AppResource& AppResourceManager::FindUnique(const std::string& host_res) const
{
	host_resource host_resname = ParseResourceName(host_res);
	std::vector<AppResource*> res;
	const std::string& dev_tag = utility::DevTag();

	std::for_each( std::begin(resources_), std::end(resources_), [&res, &host_resname, &dev_tag](const std::unique_ptr<AppResource>& entry)
	{
		if( std::get<0>(host_resname).empty() )
		{
			if( entry->name == std::get<1>(host_resname) &&
				entry->dev_tag == dev_tag )
				res.push_back(entry.get());
		}else
		{
			if( entry->host == std::get<0>(host_resname) &&
				entry->name == std::get<1>(host_resname) &&
				entry->dev_tag == dev_tag )
				res.push_back(entry.get());
		}
	});

	return res.size() == 1 ? *res[0] : bad_resource;
}

std::vector<AppResource> AppResourceManager::Find(const std::string& name, const std::string& dev_tag, const std::string& host) const
{
	std::vector<AppResource> res;
	std::for_each( std::begin(resources_), std::end(resources_), [&res, &name, &dev_tag, &host](const std::unique_ptr<AppResource>& entry)
	{
		if( entry->name == name && entry->dev_tag == dev_tag && entry->host == host )
			res.push_back(*entry);
	});
	return res;
}

std::vector<AppResource> AppResourceManager::Find(const std::string& name) const
{
	host_resource host_resname = ParseResourceName(name);
	std::vector<AppResource> res;
	const std::string& dev_tag = utility::DevTag();

	std::for_each( std::begin(resources_), std::end(resources_), [&res, &host_resname, &dev_tag](const std::unique_ptr<AppResource>& entry)
	{
		if( std::get<0>(host_resname).empty() )
		{
			if( entry->name == std::get<1>(host_resname) &&
				entry->dev_tag == dev_tag )
				res.push_back(*entry);
		}else
		{
			if( entry->host == std::get<0>(host_resname) &&
				entry->name == std::get<1>(host_resname) &&
				entry->dev_tag == dev_tag)
				res.push_back(*entry);
		}
	});
	return res;
}

const AppResource& AppResourceManager::FindNodeRelay(const std::string& node) const
{
	const std::string& dev_tag = utility::DevTag();

	auto iter = std::find_if( std::begin(resources_), std::end(resources_), [&node, &dev_tag](const std::unique_ptr<AppResource>& entry)->bool
	{
		return entry->node == node && entry->type == "sys_relay" && entry->dev_tag == dev_tag;
	});
	return iter != resources_.end() ? *iter->get() : bad_resource;
}

AppResourceManager::host_resource AppResourceManager::ParseResourceName(const std::string& res_name)
{
	host_resource res;
	size_t pos = res_name.find(':');
	if( pos == res_name.size()-1 ) // host:
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT, 
							"ParseResourceName", 
							"Bad resource name format: " + res_name
							);

	if( pos != std::string::npos )
	{
		std::get<0>(res) = res_name.substr(0, pos);
		std::get<1>(res) = res_name.substr(pos+1); 
	}else
	{
		std::get<1>(res) = res_name;
	}

	return res;
}

void TSystem::layinfo::RegisterApp( AppInfo& info, AppResourceManager& mgt )
{
	std::vector<AppResource> res = mgt.Find(info.name(), utility::DevTag(), utility::host());
	if( res.empty() )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT, 
							"RegisterApp",
							"Can't find resource entry"
							);
	}

	static AppRegistration registration;
	auto iter = std::find_if( std::begin(res), std::end(res), [](const AppResource& res)
	{
		return registration.CheckAndRegisterPid(res.pid);
	});

	if( iter != res.end() )
	{
		info.node(iter->node);
		info.pid(iter->pid);
		info.port(iter->port);	
	}else
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT, 
							"RegisterApp",
							"No available entry"
							);
					
	}
}
