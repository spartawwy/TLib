#ifndef TSYSTEM_FAKE_POSITION_H_
#define TSYSTEM_FAKE_POSITION_H_

#include <memory>
#include <unordered_map>

#include "TLib/core/tsystem_core_common.h"
#include "TLib/ep/tsystem_ecp_msg.pb.h"


namespace TSystem
{
namespace position
{
	class PosInfo;

	struct FakePosition
	{
		explicit FakePosition(int conid, int user_id, int longdate);

		operator bool() const;

		// update fake position
		void UpdateFakePosition(int qty, Mode type);

		int conid;			// contract id
		int user_id;		// user id
		int longdate;		// date

		int fake_adjust_;	// fake position counted as normal position
		int fake_position_; // fake position counted for position distortion only
	};


	//--------------------------------
	// manager of a group of  
	//--------------------------------
	class FakePositionManager
	{
	public:
		// constructor
		explicit FakePositionManager();
		FakePositionManager(FakePositionManager&& in);
		
		// operators
		FakePositionManager& operator=( FakePositionManager&& in);

		// find position interface
		const FakePosition* FindPosition(int conid) const;
		FakePosition* FindPosition(int conid);

		// init position holder
		template<typename IteratorType, typename ParserType>
		void InitExchangePosition(int user_id, int longdate, IteratorType first, IteratorType last, ParserType&& parser )
		{
			while( first != last )
				fake_position_holder_.emplace_back(new FakePosition(parser(*first++), user_id, longdate));
		}

		const std::vector<std::unique_ptr<FakePosition>>& AllFakePositions() const { return fake_position_holder_; }
		std::vector<std::unique_ptr<FakePosition>>& AllFakePositions() { return fake_position_holder_; }

	private:

		void MapFakePositions();

		// query by contract id
		std::unordered_map<int, FakePosition*>		id_map_;

		// fake position holder
		std::vector<std::unique_ptr<FakePosition>>	fake_position_holder_;

		friend class PosInfo;
	};

	// load dev_tag() laydb, throw TException
	void LoadAppFakePosition(PosInfo& pos_info, FakePositionManager& mgt, int longdate, int user_id = 0 );
}
}


#endif //TSYSTEM_FAKE_POSITION_H_