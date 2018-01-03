#include "tsystem_fake_position.h"
#include "tsystem_posinfo.h"

using namespace TSystem;
using namespace TSystem::position;

FakePosition::FakePosition(int conid, int user_id, int longdate)
	: conid(conid), user_id(user_id), longdate(longdate)
{
	fake_adjust_ = fake_position_ = 0;
}

FakePosition::operator bool() const
{
	return conid != 0 && longdate > 0;
}

void FakePosition::UpdateFakePosition(int qty, Mode type)
{
	switch(type)
	{
	case ADJUST: 
		fake_adjust_ += qty;
		break;
	case POSITION:
		fake_position_ += qty;
		break;
	default: 
		break;
	}
}

FakePositionManager::FakePositionManager()
	: id_map_(1024)
	, fake_position_holder_()
{
	fake_position_holder_.reserve(1024);
}

FakePositionManager::FakePositionManager(FakePositionManager&& in)
{
	id_map_ = std::move(in.id_map_);
	fake_position_holder_ = std::move(in.fake_position_holder_);
}

FakePositionManager& FakePositionManager::operator=(FakePositionManager&& in)
{
	if( this != &in )
	{
		id_map_ = std::move(in.id_map_);
		fake_position_holder_ = std::move(in.fake_position_holder_);
	}

	return *this;
}

const FakePosition* FakePositionManager::FindPosition(int conid) const
{
	auto iter = id_map_.find(conid);
	return iter != id_map_.end() ? iter->second : nullptr;
}

FakePosition* FakePositionManager::FindPosition(int conid)
{
	auto iter = id_map_.find(conid);
	return iter != id_map_.end() ? iter->second : nullptr;
}

void FakePositionManager::MapFakePositions()
{
	// sort
	std::sort( std::begin(fake_position_holder_), std::end(fake_position_holder_)
		, [](const std::unique_ptr<FakePosition>& left, const std::unique_ptr<FakePosition>& right)->bool
	{ return left->conid < right->conid; });

	// remove duplicated
	auto iter = std::unique( std::begin(fake_position_holder_), std::end(fake_position_holder_)
		, [](const std::unique_ptr<FakePosition>& left, const std::unique_ptr<FakePosition>& right)->bool
	{ return left->conid == right->conid; });

	fake_position_holder_.resize( std::distance(std::begin(fake_position_holder_), iter) );

	// map
	id_map_.clear();
	std::for_each(std::begin(fake_position_holder_), std::end(fake_position_holder_)
		, [this](const std::unique_ptr<FakePosition>& entry)
	{
		id_map_.insert( std::make_pair(entry->conid, entry.get()));
	});
}

void position::LoadAppFakePosition(PosInfo& pos_info, FakePositionManager& mgt, int longdate, int user_id)
{
	pos_info.LoadFakePositionHelper(mgt, user_id, longdate);
}
