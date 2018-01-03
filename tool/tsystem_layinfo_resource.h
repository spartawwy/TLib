// by Xixin Silas Cheng

#ifndef TSYSTEM_LAYINFO_RESOURCE_H_
#define TSYSTEM_LAYINFO_RESOURCE_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace TSystem
{
	class AppInfo;

namespace layinfo
{
	// struct to load resource information
	struct AppResource
	{
		AppResource();

		operator bool() const;

		std::string  name;
		std::string  type;
		std::string  dev_tag;
		std::string	 node;
		std::string  host;
		std::string  domain;
		unsigned short	port;
		unsigned short  pid;
		unsigned short	raid_id;
	};

	// manager of all app resources
	class AppResourceManager
	{
	public:
		
		AppResourceManager();
		AppResourceManager(AppResourceManager&& in);
		AppResourceManager& operator=(AppResourceManager&& in);

		//-----------------------
		// add resource related
		//-----------------------

		// add resource, throw TException when fail
		void RegisterResource(const AppResource& resource);

		// map resource, must be called after all the registrations are done
		void MapResource();

		//------------
		// find unqiue
		//------------

		// find unique by pid
		const AppResource& FindUnique(unsigned short pid) const;

		// find unique by host:res / res with current dev_tag
		// return bad_resource if not found or not unique
		// throw TException when fail to parse
		const AppResource& FindUnique(const std::string& host_res) const;

		//---------
		// find
		//---------

		// find resource by (name, dev_tag, host), there can be multiple lines 
		// with the same tag, therefore return vector of AppResource
		std::vector<AppResource> Find(const std::string& name, const std::string& dev_tag, const std::string& host) const;

		// find resource by name with current dev_tag
		// throw TException when fail to parse name
		std::vector<AppResource> Find(const std::string& name) const;

		//-----------
		// find other
		//-----------

		// find relay of the node with current dev_tag
		const AppResource& FindNodeRelay(const std::string& node) const;

		// all resources
		const std::vector<std::unique_ptr<AppResource>>& AllResources() const { return resources_; }

	private:	

		// (host, resource_name) pair
		typedef std::tuple<std::string, std::string> host_resource;

		// parse resource name (in format of host:res) into (host, res) pair
		// throw TException if bad format
		static host_resource ParseResourceName(const std::string& res_name);

		// will be called on realtime, use unordered_map indexed by pid
		std::unordered_map<unsigned short, const AppResource*>	pid_maps_;

		// place holder for fast looping
		std::vector<std::unique_ptr<AppResource>>				resources_;

		// the default bad resource
		static AppResource                      bad_resource;
	};

	// free functions

	// register app  
	// throw exception when fail
	void RegisterApp( AppInfo& app, AppResourceManager& mgt );
}
}

#endif //TSYSTEM_LAYINFO_RESOURCE_H_