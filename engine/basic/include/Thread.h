#ifndef _Thread_H_
#define _Thread_H_
#pragma 

#include <boost/assert.hpp>
#include <thread>
#include <condition_variable>
#include <mutex>

#ifdef KLAYGE_TS_LIBRARY_OPTIONAL_SUPPORT
#include <experimental/optional>
#else
#ifdef AIR_COMPLER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif // AIR_COMPLER_CLANG

#include <boost/optional.hpp>
#ifdef AIR_COMPLER_CLANG
#pragma clang diagnostic pop
#endif
namespace std
{
	namespace experimental
	{
		using boost::optional;
	}
}
#endif

#include <thread>
#include <mutex>

#include <exception>
#include <vector>
#include <functional>

namespace Air
{
	struct void_t
	{
		typedef void_t type;
	};

	typedef std::thread::id thread_id;

	inline thread_id threadof(int)
	{
		return std::this_thread::get_id();
	}

	template<typename Joiner>
	inline thread_id threadof(Joiner const & j)
	{
		return j.get_thread_id();
	}

	class bad_join : public std::exception
	{
	public:
		const char* what() const AIR_NOEXCEPT
		{
			return "bad join";
		}
	};

	template <typename result_type>
	class joiner_impl_base
	{
	public:
		typedef std::experimental::optional<
			typename std::conditional<std::is_same<result_type, void>::value, 
			void_t, result_type>::type
		> result_opt;

		typedef typename std::conditional<std::is_same<result_type, void>::value, result_type, typename std::add_lvalue_reference<result_type>::type>::type const_result_type_ref;
	public:
		joiner_impl_base()
			:mJoined(false)
		{

		}
		virtual ~joiner_impl_base()
		{

		}
		const_result_type_ref join()
		{
			try
			{
				std::lock_guard<std::mutex> locker(mJoinerMutex);
				if (!mJoined)
				{
					if (threadof(*this) == threadof(0))
					{
						throw bad_join();
					}
					this->do_join();
					this->do_join();
					mJoined = true;
				}
			}
			catch (...)
			{
				throw bad_join();
			}
		}
		void detach()
		{
			this->do_detach();
		}
		thread_id get_thread_id() const 
		{
			return mId;
		}
	private:
		virtual void do_join() = 0;
		virtual void do_detach() = 0;
	protected:
		std::shared_ptr<result_opt> mResult;
		volatile bool				mJoined;
		std::mutex					mJoinerMutex;
		thread_id					mId;
	};


	template <typename ResultType>
	class joiner
	{
		typedef joiner_impl_base<ResultType> joiner_base_t;
	public:
		typedef typename joiner_base_t::const_result_type_ref const_result_type_ref;
		thread_id get_thread_id() const
		{
			return mHandle ? mHandle->get_thread_id() : threadof(0);
		}
	public:
		joiner()
		{

		}
		explicit joiner(std::shared_ptr<joiner_impl_base<ResultType>> const &handle)
			:mHandle(handle)
		{

		}
		joiner(joiner const & other)
			:mHandle(other.mHandle)
		{

		}

		joiner& operator = (joiner const & other)
		{
			mHandle = other.mHandle;
			return *this;
		}
		const_result_type_ref operator()()
		{
			if (!mHandle)
			{
				throw bad_join();
			}
			return mHandle->join();
		}
		void detach()
		{
			mHandle->detach();
		}
	public:
		friend bool operator == (joiner<ResultType> const &lhs, joiner<ResultType> const &rhs)
		{
			return lhs.mHandle.get() == rhs.mHandle.get();
		}
		friend bool operator != (joiner<ResultType> const &lhs, joiner<ResultType> const &rhs)
		{
			return !(lhs == rhs);
		}
		friend bool operator<(joiner<ResultType> const & lhs, joiner<ResultType> const & rhs)
		{
			return lhs.handle_.get() < rhs.handle_.get();
		}

		friend bool operator>(joiner<ResultType> const & lhs, joiner<ResultType> const & rhs)
		{
			return rhs < lhs;
		}

		friend bool operator<=(joiner<ResultType> const & lhs, joiner<ResultType> const & rhs)
		{
			return !(rhs < lhs);
		}

		friend bool operator>=(joiner<ResultType> const & lhs, joiner<ResultType> const & rhs)
		{
			return !(lhs < rhs);
		}
	private:
		std::shared_ptr<joiner_impl_base<ResultType>> mHandle;
	};


	namespace detail
	{
		// This is the function executed by the underlying thread system that calls the user supplied Threadable object
		//  and takes care of exception handling
		template <typename Threadable, typename JoinerImpl>
		class threaded
		{
			typedef threaded<Threadable, JoinerImpl>				threaded_t;
			typedef typename std::result_of<Threadable()>::type		result_t;
			typedef JoinerImpl										joiner_impl_t;
			typedef typename JoinerImpl::result_opt					result_opt;
			typedef std::experimental::optional<void_t>				void_optional_t;

			//Helper function to construct the optional from the
			//return value and handle void return types
			template <typename MainFunctionHolder, typename OptionalOut>
			static void construct_result(MainFunctionHolder& in, OptionalOut& out)
			{
				out = in.main_();
			}

			template <typename MainFunctionHolder>
			static void construct_result(MainFunctionHolder& in, void_optional_t& out)
			{
				in.main_();
				out = void_t();
			}

		public:
			threaded(Threadable const & main,
				std::shared_ptr<result_opt> const & result)
				: main_(main), mResult(result)
			{
			}

			// This function is the function executed by the underlying thread system. It takes ownership of
			//  the threaded object, calls user's Threadable
			static void needle(std::shared_ptr<threaded_t> const & that)
			{
				// Call Threadable
				try
				{
					that->construct_result(*that, *that->mResult);
				}
				catch (...)
				{
					// If an exception is thrown, result_opt is not constructed, and this can be detected
					//  using optional's interface by joiners
				}
			}

		private:
			Threadable					main_;
			std::shared_ptr<result_opt>	mResult;
		};
	}

	// A threader class that whose operator() launches a threadable	object in a new thread
	class threader
	{
		// This is the implementation of joiner functions created by "create_thread" and "class threader" threaders.
		template <typename result_type>
		class joiner_simple_thread_impl : public joiner_impl_base<result_type>
		{
		public:
			joiner_simple_thread_impl(std::shared_ptr<typename joiner_impl_base<result_type>::result_opt> const & result_op,
				std::function<void()> const & func)
				: thread_(func)
			{
				joiner_impl_base<result_type>::mResult = result_op;
				joiner_impl_base<result_type>::mId = thread_.get_id();
			}

		private:
			void do_join()
			{
				thread_.join();
			}

			void do_detach()
			{
				thread_.detach();
			}

		private:
			std::thread thread_;
		};

	public:
		// Launches threadable function in a new thread. Returns a joiner that can be used to wait thread completion.
		template <typename Threadable>
		joiner<typename std::result_of<Threadable()>::type> operator()(Threadable const & function)
		{
			typedef typename std::result_of<Threadable()>::type		result_t;
			typedef joiner<result_t>								joiner_t;
			typedef joiner_simple_thread_impl<result_t>				joiner_impl_t;
			typedef typename joiner_impl_t::result_opt				result_opt;
			typedef detail::threaded<Threadable, joiner_impl_t>		threaded_t;

			std::shared_ptr<result_opt> myreturn = MakeSharedPtr<result_opt>();
			std::shared_ptr<threaded_t> mythreaded = MakeSharedPtr<threaded_t>(function, myreturn);
			std::shared_ptr<joiner_impl_base<result_t>> myjoiner_data = MakeSharedPtr<joiner_impl_t>(myreturn,
				bind(&threaded_t::needle, mythreaded));

			return joiner_t(myjoiner_data);
		}
	};

	// Threader function that creates a new thread to execute the Threadable. Just creates a temporary object of class threader
	//  and uses operator()(Threadable)
	template <typename Threadable>
	inline joiner<typename std::result_of<Threadable()>::type> create_thread(Threadable const & function)
	{
		return threader()(function);
	}


	// This Threader class creates a pool of threads that can be reused for several Threadable object executions.
	//  If the thread pool runs out of threads it creates more. The user can specify the minimum and maximum
	//  number of pooled threads.
	class thread_pool
	{
		class thread_pool_common_data_t;

		// A class used to synchronize joining with a thread launched from a thread pool and notifying to pooled thread
		//  that it can be recycled
		class thread_pool_join_info
		{
		public:
			thread_pool_join_info();

			// Used by joiner to wait until the thread from the pool completes its task
			void join();

			// Used by a thread from the pool to wake up a blocked joiner()
			void notify_join();

			// Used by the last joiner to notify to the thread from the pool that it can be recycled for the next task
			void recycle();

			// Used by the thread from the pool to wait until the last joiner is destroyed and notifies that the thread
			//  from the pool can be recycled
			void wait_recycle();

		private:
			volatile bool     join_now_;
			volatile bool     can_recycle_thread_;
			std::condition_variable  cond_;
			std::mutex      join_mut_;
		};

		// A class used to storage information of a pooled thread. Each object of this class represents a pooled thread.
		//  It also has mechanisms to notify the pooled thread that it has a work to do. It also offers notification
		//  to definitively tell to the thread that it should die.
		struct thread_pool_thread_info
		{
			explicit thread_pool_thread_info(std::shared_ptr<thread_pool_common_data_t> const & pdata);

			// Wakes up a pooled thread assigning a task to it
			template <typename Threadable>
			void wake_up(Threadable const & func, std::shared_ptr<thread_pool_join_info> const & pthpool_join_info)
			{
				// Assign function to execute and joiner notifier
				thpool_join_info_ = pthpool_join_info;
				func_ = func;
				{
					std::lock_guard<std::mutex> lock(wake_up_mut_);
					wake_up_ = true;
					wake_up_cond_.notify_one();
				}
			}

			// Wakes up a pooled thread saying it should die
			void kill();

			thread_id get_thread_id() const
			{
				return mId;
			}

			void set_thread_id(thread_id id)
			{
				mId = id;
			}

			std::shared_ptr<thread_pool_join_info> thpool_join_info_;
			std::function<void()>	func_;
			bool					wake_up_;
			std::mutex				wake_up_mut_;
			std::condition_variable	wake_up_cond_;
			std::weak_ptr<thread_pool_common_data_t>	data_;
			thread_id				mId;
		};

		// A class used to storage information of the thread pool. It stores the pooled thread information container
		//  and the functor that will envelop users Threadable to return it to the pool.
		class thread_pool_common_data_t : public std::enable_shared_from_this<thread_pool_common_data_t>
		{
			// This functor is the functor that implements thread pooling logic. Waits until someone fills
			//  the func_ and thpool_join_info_ using the wake_up(...) function of the pooled thread's info.
			class wait_function
			{
			public:
				// Stores a std::shared_ptr with the data that holds the thread pool.
				explicit wait_function(std::shared_ptr<thread_pool_thread_info> const & info);

				// This is the thread pool loop. Waits for task, executes it and if there are not enough threads
				//  in the pool, enqueues itself again in the queue.
				void operator()();

			private:
				std::shared_ptr<thread_pool_thread_info> info_;
			};

		public:
			typedef std::vector<std::shared_ptr<thread_pool_thread_info>> thread_info_queue_t;

			thread_pool_common_data_t(size_t num_min_cached_threads, size_t num_max_cached_threads);

			// Creates and adds more threads to the pool. Can throw
			static void add_waiting_threads(std::shared_ptr<thread_pool_common_data_t> const & pdata, size_t number);

			// Creates and adds more threads to the pool. This function does not lock the pool mutex and that be
			//  only called when we externally have locked that mutex. Can throw
			static void add_waiting_threads_no_lock(std::shared_ptr<thread_pool_common_data_t> const & data, size_t number);

			// Notifies all pooled threads that this is the end
			void kill_all();

			std::mutex& mut()
			{
				return mut_;
			}

			thread_info_queue_t& threads()
			{
				return threads_;
			}

			size_t num_min_cached_threads() const
			{
				return num_min_cached_threads_;
			}
			void num_min_cached_threads(size_t num);

			size_t num_max_cached_threads() const
			{
				return num_max_cached_threads_;
			}
			void num_max_cached_threads(size_t num)
			{
				num_max_cached_threads_ = num;
			}

		private:
			// Shared data between thread_pool, all threads and joiners
			size_t						num_min_cached_threads_;
			size_t						num_max_cached_threads_;
			std::mutex					mut_;
			bool						general_cleanup_;
			thread_info_queue_t			threads_;
			threader					threader_;
		};

		// This is the implementation of joiner functions created by "create_thread" and "class threader" threaders.
		template <typename result_type>
		class joiner_thread_pool_impl : public joiner_impl_base<result_type>
		{
			friend class thread_pool;

		public:
			template <typename Threadable>
			joiner_thread_pool_impl(std::shared_ptr<thread_pool_common_data_t> const & data,
				std::shared_ptr<typename joiner_impl_base<result_type>::result_opt> const & result_op,
				Threadable const & func)
				: thread_pool_join_info_(MakeSharedPtr<thread_pool_join_info>())
			{
				joiner_impl_base<result_type>::mResult = result_op;

				std::shared_ptr<thread_pool_thread_info> th_info;
				std::lock_guard<std::mutex> lock(data->mut());

				// If there are no threads, add more to the pool
				if (data->threads().empty())
				{
					thread_pool_common_data_t::add_waiting_threads_no_lock(data, 1);
				}
				th_info = data->threads().front();
				joiner_impl_base<result_type>::mId = th_info->get_thread_id();
				data->threads().erase(data->threads().begin());
				th_info->wake_up(func, thread_pool_join_info_);
			}

			~joiner_thread_pool_impl()
			{
				try
				{
					thread_pool_join_info_->recycle();
				}
				catch (...)
				{
				}
			}

		private:
			void do_join()
			{
				thread_pool_join_info_->join();
			}

			void do_detach()
			{
			}

		private:
			std::shared_ptr<thread_pool_join_info> thread_pool_join_info_;
		};

	public:
		thread_pool(size_t num_min_cached_threads, size_t num_max_cached_threads);
		~thread_pool();

		// Launches threadable function in a new thread. If there is a pooled thread available, reuses that thread.
		template <typename Threadable>
		joiner<typename std::result_of<Threadable()>::type> operator()(Threadable const & function)
		{
			typedef typename std::result_of<Threadable()>::type		result_t;
			typedef joiner<result_t>								joiner_t;
			typedef joiner_thread_pool_impl<result_t>				joiner_impl_t;
			typedef typename joiner_impl_t::result_opt				result_opt;
			typedef detail::threaded<Threadable, joiner_impl_t>		threaded_t;

			std::shared_ptr<result_opt> myreturn = MakeSharedPtr<result_opt>();
			std::shared_ptr<threaded_t> mythreaded = MakeSharedPtr<threaded_t>(function, myreturn);
			std::shared_ptr<joiner_impl_base<result_t>> myjoiner_data = MakeSharedPtr<joiner_impl_t>(data_,
				myreturn, std::bind(&threaded_t::needle, mythreaded));

			return joiner_t(myjoiner_data);
		}

		size_t num_min_cached_threads() const
		{
			return data_->num_min_cached_threads();
		}
		void num_min_cached_threads(size_t num)
		{
			data_->num_min_cached_threads(num);
		}

		size_t num_max_cached_threads() const
		{
			return data_->num_max_cached_threads();
		}
		void num_max_cached_threads(size_t num)
		{
			data_->num_max_cached_threads(num);
		}

	private:
		std::shared_ptr<thread_pool_common_data_t> data_;
	};
}

#endif