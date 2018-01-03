#ifndef TSYSTEM_DATA_SERVER
#define TSYSTEM_DATA_SERVER

#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>

#include "TLib/core/tsystem_connection.h"
#include "TLib/core/tsystem_task_service.h"

namespace TSystem
{
namespace communication
{
	// class of Topic Data
	// a topic data is a data source for tcp connections allowing data pushing.
	// Topic data includes data queue and destinations, one connection can be
	// added into topic data and receving pushed data from it.
	//
	// in most case, when you do not need to keep the complete data queue, call
	// AsyncUpdateData(), which only keeps track of / send the latest data.
	//
	// in case you need to keep the complete data history and allow client connection
	// to receive complete data history, call AsyncUpdateData(), which pushes data into
	// data queue, and backfill all data to client connection in batch.
	// 
	// YOU CAN ONLY CALL AsyncUpdateData() OR AsyncUpdateData(), NOT BOTH
	//
	// it also supports sending once, in which case it sends the current snapshot of 
	// its internal data to the destination without continouly pushing data
	//
	// each Topic data has its own execution strand, which gives lockfree
	// thread safty.
	class TopicData
	{
	public:

		// when a new connection coming, data position in the queue it begins
		// to receive data
		enum class DataStartPos: char
		{
			FIRST  = 'h',		// from first data in the data queue
			LAST   = 't',       // from last data in the data queue
			LATEST = 'l'     // from next data pushed/updated into data queue
		};

		explicit TopicData(const TSystem::TaskPool& pool);
		~TopicData();

		// moving constructor
		TopicData( TopicData&& in);

		// add new data into queue and start sending procedure
		void AsyncAddData( CodedDataPtr&& data );
		void AsyncAddData( const CodedDataPtr& data );

		// add destination, will start sending procedure if pos = first / last
		void AsyncAddDestination( std::shared_ptr<Connection>&& pconn, DataStartPos pos );
		void AsyncAddDestination( const std::shared_ptr<Connection>& pconn, DataStartPos pos);

		// remove destination
		void AsyncRemoveDestination( int conid);

		// update new data (old last data is replaced) and send to all
		void AsyncUpdateData( CodedDataPtr&& data );
		void AsyncUpdateData( const CodedDataPtr& data);

		// send latest to one dest
		void AsyncSendOne( std::shared_ptr<Connection>&& pconn, DataStartPos pos);
		void AsyncSendOne( const std::shared_ptr<Connection>& pconn, DataStartPos pos);

	private:
		// forward destination
		// (next to be sent offset, connection)
		typedef std::tuple<std::size_t, std::shared_ptr<Connection>>	DestinationType;
		typedef std::vector<DestinationType>	ForwardDests;

		// add data into queue
		void AddData(  CodedDataPtr& data );

		// send queue
		void SendData();

		// add destination
		void AddDestination( std::shared_ptr<Connection>& pconn, DataStartPos pos );

		// remove destination
		void RemoveDestination(int conid);

		// update data and send to all
		void UpdateData( CodedDataPtr& data);

		// send data once
		void SendOne( std::shared_ptr<Connection>& pconn, DataStartPos pos );


		//-------------------
		// utility functions
		//-------------------

		// send data queue to one connection, return if connection's sending progress is done
		bool SendDataToOne(DestinationType& dest);
		
		// wrapper of SendDataToOne, but check if connection is connected before send
		void CheckAndSendDataToOne(DestinationType& dest);

		// strand of this topic data, 
		TSystem::TaskStrand		topic_data_strand_;

		// latest data
		std::deque<CodedDataPtr>	all_data_;

		// forward destinations
		ForwardDests    dests_;		 
	};

	// throw TException if c out of enum range
	TopicData::DataStartPos IntToDataStartPos(int c);

	// class of a Topic
	// a topic is something like a variable name, for example "bidask", "forward"
	// each Topic owns topic data according to keys, like bidask(topic) of 510050(key).
	// the best practice of topic is to initiate all keys before using it, but it also
	// supports adding key on fly, which you need to set all_add_key() to true (false by default)
	class Topic
	{
	public:
		Topic();
		~Topic();

		// initiate topic data for given keys, thread safe
		TopicData& AddTopicData( const std::string& key, const TaskPool& pool);

		// find a topic data
		TopicData* Find(const std::string& key);

		// add destination for all TopicData
		void AddDestinationForAll( const std::shared_ptr<Connection>& pconn, TopicData::DataStartPos pos );

		// remove destination from all TopicData
		void RemoveDestinationFromAll( int connid );

		// send one for all topic data
		void SendOneForAll(  const std::shared_ptr<Connection>& pconn, TopicData::DataStartPos pos );

		// allow add_key
		bool allow_add_key() const { return allow_add_key_; }
		void allow_add_key(bool b) { allow_add_key_ = b; }

	private:
		// (key, TopicData)
		std::unordered_map<std::string, std::unique_ptr<TopicData>> topic_data_;
		std::mutex													topic_data_mux_;
		
		// allow add key on fly
		bool														allow_add_key_;

		// destination of subsribe all
		std::vector<std::shared_ptr<Connection>>					all_key_subscriber_;
	};

	// class of TopicServer
	// TopicServer should be initated with topics
	class TopicServer
	{
	public:
		
		// constructor
		TopicServer();

		// destructor
		~TopicServer();
	
		// initiate
		Topic& AddTopic(unsigned short topic_id, const TaskPool& pool);

		// initiate topic data 
		TopicData& AddTopicData( unsigned short topic_id, const std::string& key, const TaskPool& pool);

		// find topic data
		TopicData* Find(unsigned short topic_id, const std::string& key);

		// find topic
		Topic*    Find(unsigned short topic_id);

        std::unordered_map<unsigned short, std::unique_ptr<Topic>>* topics(){ return &topics_;}

	private:

		// (topic id, Topic)
		std::unordered_map<unsigned short, std::unique_ptr<Topic>> topics_;
	};
}
}

#endif // TSYSTEM_DATA_SERVER