/************************************************
 * Author : mark
 * Email : hzypp1995@gmail.com
 * Lab : Zhejiang University ZJUNlict
 * Introduction : 
 *     (Semaphore_base)────┐             ┌──────(ZSData_base) 
 *        /        \       |             |     /      \    
 *    (SemaO)    (SemaM)  (ZSSemaData_Base)  (ZSData) (ZSDataQueue)
 *       |          |          /     \         |            |
 *       |          ├(ZSemaData NoFS)  (ZSemaData FS)       |
 *       └──────────│──────────────────────────┘            |
 *                  └───────────────────────────────────────┘
 ***************************************************/

#ifndef __ZSS_PLUGIN_H__
#define __ZSS_PLUGIN_H__
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <cstring>
#include <condition_variable>
#include <atomic>

class Semaphore_base {
public:
    Semaphore_base(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " Semaphore_base constructor" << std::endl;
        #endif
    }
    virtual ~Semaphore_base(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " Semaphore_base destructor" << std::endl;
        #endif
    }
    virtual void signal(unsigned int c) = 0;
    virtual void notify_current() = 0;
    virtual bool wait(int idx) = 0;
    virtual bool try_wait() = 0;
};
class SemaphoreM:public Semaphore_base {
public:
    SemaphoreM(long count = 0)
        : count_(count) {
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreM constructor" << std::endl;
        #endif
    }
    SemaphoreM(const SemaphoreM& s):count_(0){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreM copy constructor" << std::endl;
        #endif
    }
    virtual ~SemaphoreM(){
        notify_current();
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreM destructor" << std::endl;
        #endif
    }
    virtual void notify_current() override{
        std::unique_lock<std::mutex> lock(mutex_);
        count_ = 99999;
        cv_.notify_all();
    }
    virtual void signal(unsigned int c = 1) override {
        std::unique_lock<std::mutex> lock(mutex_);
        count_+=c;
        cv_.notify_one();
    }
    virtual bool wait(int idx) override {
        std::unique_lock<std::mutex> lock(mutex_);
        if(idx == 0){
            cv_.wait(lock, [=] { return count_ > 0; });
            return true;
        }
        auto res = cv_.wait_for(lock, idx*std::chrono::milliseconds(100), [=] { return count_ > 0; });
        if(res) {
            --count_;
        }
        return res;
    }
    virtual bool try_wait() override {
        std::unique_lock<std::mutex> lock(mutex_);
        if(count_ > 0){
            --count_;
            return true;
        }
        return false;
    }
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    long count_;
};

class SemaphoreO:public Semaphore_base {
public:
    SemaphoreO(long count = 0)
        : count_(count) {
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreO constructor" << std::endl;
        #endif
    }
    SemaphoreO(const SemaphoreO& s):count_(0){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreO copy constructor" << std::endl;
        #endif
    }
    virtual ~SemaphoreO(){
        notify_current();
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " SemaphoreO destructor" << std::endl;
        #endif
    }
    virtual void signal(unsigned int c = 1) override {
        std::unique_lock<std::mutex> lock(mutex_);
        count_=c;
        cv_.notify_one();
    }
    virtual void notify_current() override{
        std::unique_lock<std::mutex> lock(mutex_);
        count_ = 99999;
        cv_.notify_all();
    }
    virtual bool wait(int idx) override {
        std::unique_lock<std::mutex> lock(mutex_);
        if(idx == 0){
            cv_.wait(lock, [=] { return count_ > 0; });
            --count_;
            return true;
        }
        auto res = cv_.wait_for(lock, idx*std::chrono::milliseconds(100), [=] { return count_ > 0; });
        if(res){
            --count_;
        }
        return res;
    }
    virtual bool try_wait() override {
        std::unique_lock<std::mutex> lock(mutex_);
        if(count_ > 0){
            --count_;
            return true;
        }
        return false;
    }
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    long count_;
};
class ZSData;
class ZSData_base{
public:
    ZSData_base(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSData_base constructor" << std::endl;
        #endif
    }
    virtual ~ZSData_base(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSData_base destructor" << std::endl;
        #endif
    }
    virtual void popTo(ZSData&) = 0;
    virtual void store(const void* const data,unsigned long size) = 0;
};

// thread unsafe base node
class ZSDataNode{
public:
    ZSDataNode():_last(nullptr),_next(nullptr),_data(nullptr),_capacity(0),_size(0){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSDataNode constructor" << std::endl;
        #endif
    }
    virtual ~ZSDataNode(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSDataNode destructor" << std::endl;
        #endif
        if(_capacity > 0){
            free(_data);
        }
    }
    virtual void resize(const unsigned long size){
        if(size > _capacity){
            if(_data != nullptr){
                free(_data);
            }
            _data = malloc(size);
            _capacity = size;
        }
        _size = size;
    }
    ZSDataNode* _last;
    ZSDataNode* _next;
    void* _data;
    unsigned long _capacity;
    unsigned long _size;
};
class ZSData:public ZSData_base,public ZSDataNode{
public:
    ZSData():ZSDataNode(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSData constructor" << std::endl;
        #endif
    }
    ZSData(const ZSData& data){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSData copy constructor" << std::endl;
        #endif
        store(data);
    }
    virtual ~ZSData(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSData destructor" << std::endl;
        #endif
    }
    // self thread-safe
    virtual int size() const {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _size;
    }
    virtual void popTo(ZSData& p){
        std::unique_lock<std::shared_mutex> lock(_mutex);
        p.store(this->data(),this->_size);
        resize(0);
    }
    virtual void copyTo(ZSData* p){
        std::shared_lock<std::shared_mutex> lock(_mutex);
        p->store(this->data(),this->_size);
    }
    virtual void store(const ZSData& data){
        store(data.data(),data._size);
    }
    virtual void store(const ZSData* data){
        store(data->data(),data->_size);
    }
    virtual void store(const void* const data,unsigned long size) override{
        std::unique_lock<std::shared_mutex> lock(_mutex);
        resize(size);
        if(size > 0)
            memcpy(_data,data,size);
    }
    // thread-unsafe
    virtual const void* data() const { return _data; }
    virtual void* ptr() {
        return _data;
    }
protected:
    mutable std::shared_mutex _mutex;
};

class ZSDataQueue:public ZSData_base{
public:
    ZSDataQueue():_size(0),_capacity(1),_start(new ZSDataNode()),_end(_start){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSDataQueue constructor" << std::endl;
        #endif
        _start->_last = _start;
        _start->_next = _start;
    }
    virtual ~ZSDataQueue(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSDataQueue destructor" << std::endl;
        #endif
        while(_capacity>1){
            _start = _start->_next;
            delete _start->_last;
            _capacity--;
        }
        delete _start;
    }
    virtual void popTo(ZSData& p){
        std::unique_lock<std::shared_mutex> lock(_mutex);
        p.store(_start->_data,_start->_size);
        _start = _start->_next;
        _size--;
        #ifdef ZSPLUGIN_DEBUG
            std::cout << this << " ZSDataQueue popTo()" << std::endl;
            if(_size < 0){
                std::cout << this << "size < 0 after popTo(" << &p << "). _size=" << _size << ", _capacity=" << _capacity << std::endl; 
            }
        #endif
    }
    virtual void store(const ZSData& data){
        store(data.data(),data._size);
    }
    virtual void store(const ZSData* data){
        store(data->data(),data->_size);
    }
    virtual void store(const void* const data,unsigned long size){
        std::unique_lock<std::shared_mutex> lock(_mutex);
        ZSDataNode* storeNode = nullptr;
        if(_size < _capacity){
            storeNode = _end;
            _end = _end->_next;
        }else{
            storeNode = new ZSDataNode();
            storeNode->_next = _end;
            storeNode->_last = _end->_last;
            _end->_last->_next = storeNode;
            _end->_last = storeNode;
            _capacity++;
        }
        storeNode->resize(size);
        if(size > 0)
            memcpy(storeNode->_data,data,size);
        _size++;
        #ifdef ZSPLUGIN_DEBUG
            std::cout << this << " ZSDataQueue store() " << storeNode << ' ' << data << ", size=" << _size << ", capacity=" << _capacity << std::endl;
        #endif
    }
    virtual unsigned long size(){
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _size;
    }
protected:
    unsigned long _size;
    unsigned long _capacity;
    ZSDataNode* _start;
    ZSDataNode* _end;
    mutable std::shared_mutex _mutex;
};

class ZSSemaData:public ZSData_base,public Semaphore_base{
public:
    ZSSemaData(bool singleData = true):_data(nullptr),_semaphore(nullptr){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSSemaData constructor" << std::endl;
        #endif
        if(singleData){
            _data = new ZSData();
            _semaphore = new SemaphoreO();
        }else{
            _data = new ZSDataQueue();
            _semaphore = new SemaphoreM();
        }
    }
    virtual ~ZSSemaData(){
        delete _data;
        delete _semaphore;
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " ZSSemaData destructor" << std::endl;
        #endif
    }
    virtual void popTo(ZSData& p) override{
        _data->popTo(p);
    }
    virtual void store(const ZSData& data){
        store(data.data(),data._size);
    }
    virtual void store(const ZSData* data){
        store(data->data(),data->_size);
    }
    virtual void store(const void* const data,unsigned long size) override{
        _data->store(data,size);
    }
    virtual void signal(unsigned int c = 1) override{
        _semaphore->signal(c);
    }
    virtual bool wait(int idx = 1) override{
        return _semaphore->wait(idx);
    }
    virtual bool try_wait() override{
        return _semaphore->try_wait();
    }
    virtual void notify_current() override{
        return _semaphore->notify_current();
    }
private:
    ZSData_base* _data;
    Semaphore_base* _semaphore;
};

class ZSPlugin{
public:
    ZSPlugin(const std::string& name):_controlCode(0),_name(name){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " ZSPlugin constructor" << std::endl;
        #endif
    }
    ZSPlugin(const ZSPlugin&):_controlCode(0),_name("_copyed"){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " ZSPlugin copy constructor" << std::endl;
        #endif
    }
    virtual ~ZSPlugin(){
        for(auto it:_databox){
            delete (it.second);
        }
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " ZSPlugin destructor" << std::endl;
        #endif
    }
    std::string name() const{ return _name; }
    virtual void run() = 0;
    virtual void start_detach() final{
        _t = std::thread([=]{
            run();_controlCode = CONTROL_EXIT;
            #ifdef ZSPLUGIN_DEBUG
            std::cout << this << ' ' << this->_name << " has exit." << std::endl;
            #endif
        });
        _controlCode = CONTROL_RUNNING;
        _t.detach();
    }
    virtual void start_join() final{
        _t = std::thread([=]{
            run();_controlCode = CONTROL_EXIT;
            #ifdef ZSPLUGIN_DEBUG
            std::cout << this << ' ' << this->_name << " has exit." << std::endl;
            #endif
        });
        _controlCode = CONTROL_RUNNING;
        _t.join();
    }
    virtual void publish(const std::string& msg,const void* data = nullptr,const unsigned long size = 0) final{
        auto it = _subscribers.find(msg);
        if (it != _subscribers.end()){
            for(auto p:_subscribers[msg]){
                p->store(data,size);
                p->signal();
            }
        }
        if (it == _subscribers.end()){
            std::cerr << "ERROR : didn't DECLARE to PUBLISH this kind of message, check your message type : " << msg << std::endl;
        }
        response_to_control();
    }
    virtual void publish(const std::string& msg,const ZSData& data) final{
        auto it = _subscribers.find(msg);
        if (it != _subscribers.end()){
            for(auto p:_subscribers[msg]){
                p->store(data);
                p->signal();
            }
        }
        if (it == _subscribers.end()){
            std::cerr << "ERROR : didn't DECLARE to PUBLISH this kind of message, check your message type : " << msg << std::endl;
        }
        response_to_control();
    }
    virtual void receive(const std::string& msg){
        auto it = _databox.find(msg);
        if (it == _databox.end()){
            std::cerr << "ERROR : didn't DECLARE to RECEIVE this kind of message, check your message type : " << msg << std::endl;
            return;
        }
        while(!it->second->wait()){
            if(response_to_control() == CONTROL_NEED_EXIT)
                break;
        }
    }
    virtual void receive(const std::string& msg,ZSData& data) final{
        auto it = _databox.find(msg);
        if (it == _databox.end()){
            std::cerr << "ERROR : didn't DECLARE to RECEIVE this kind of message, check your message type : " << msg << std::endl;
            return;
        }
        while(!it->second->wait()){
            if(response_to_control() == CONTROL_NEED_EXIT)
                break;
        }
        it->second->popTo(data);
    }
    virtual bool try_receive(const std::string& msg,ZSData& data) final{
        auto it = _databox.find(msg);
        if (it == _databox.end()){
            std::cerr << "ERROR : didn't DECLARE to RECEIVE this kind of message, check your message type : " << msg << std::endl;
            return false;
        }
        bool res = it->second->try_wait();
        if(res){
            it->second->popTo(data);
        }
        response_to_control();
        return res;
    }
    virtual bool try_receive(const std::string& msg) final{
        auto it = _databox.find(msg);
        if (it == _databox.end()){
            std::cerr << "ERROR : didn't DECLARE to RECEIVE this kind of message, check your message type : " << msg << std::endl;
            return false;
        }
        bool res = it->second->try_wait();
        response_to_control();
        return res;
    }
    virtual void link(ZSPlugin* p,const std::string& msg) final{
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " link : " << msg << " with " << p << ' ' << p->_name << std::endl;
        #endif
        auto it = _subscribers.find(msg);
        if (it == _subscribers.end()){
            std::cerr << "ERROR : didn't DECLARE to PUBLISH message : " << msg << std::endl;
            return;
        }
        auto iit = p->_databox.find(msg);
        if(iit == p->_databox.end()){
            std::cerr << "ERROR : didn't DECLARE to STORE this kind of message, check your message type : " << msg << std::endl;
            return;
        }
        std::cout << "link : " << this->_name << " [" << msg << "]" << " -> " << p->_name << std::endl;
        it->second.push_back(iit->second);
    }
    virtual void declare_receive(const std::string& msg,bool frameSkipMode = true) final{
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " declare_receive " << msg << std::endl;
        #endif
        auto it = _databox.find(msg);
        if (it != _databox.end()){
            std::cerr << "ERROR : REDECLARE, check your message type : " << msg << std::endl;
            return;
        }
        _databox.insert(std::pair<std::string,ZSSemaData*>(msg,new ZSSemaData(frameSkipMode)));
    }
    virtual void declare_publish(const std::string& msg) final{
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << ' ' << this->_name << " declare_publish " << msg << std::endl;
        #endif
        auto it = _subscribers.find(msg);
        if (it != _subscribers.end()){
            std::cerr << "ERROR : REDECLARE_PUBLISH, check your message type : " << msg << std::endl;
            return;
        }
        _subscribers[msg] = {};
    }
    // control api
    virtual int get_status(){
        return _controlCode;
    }
    virtual void set_pause(bool pause){
        if(!pause && _statusCode == CONTROL_PAUSE)
            _restart.signal();
        _controlCode = pause ? CONTROL_NEED_PAUSE : CONTROL_NEED_RUN;
    }
    virtual void set_exit(){
        if(_statusCode == CONTROL_PAUSE)
            _restart.signal();
        _controlCode = CONTROL_NEED_EXIT;
    }
    virtual void wait_for_restart(){
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " Plugin : " << this->_name << " pause and wait for restart." << std::endl;
        #endif
        _restart.wait(0);
        #ifdef ZSPLUGIN_DEBUG
        std::cout << this << " Plugin : " << this->_name << " has restart." << std::endl;
        #endif
    }
    virtual int response_to_control(){
        if(_controlCode == CONTROL_NEED_PAUSE){
            _statusCode = CONTROL_PAUSE;
            wait_for_restart();
            _statusCode = CONTROL_RUNNING;
        }
        return _controlCode;
    }
protected:
    //! \brief controlCode
    //! \details
    //! 0 prepare
    //! 1 need run
    //! 2 need pause
    //! 3 need exit
    std::atomic<int> _controlCode = 0;
    //! \brief statusCode
    //! 0 prepare
    //! 1 running
    //! 2 pause
    //! 3 exit
    std::atomic<int> _statusCode = 0;
public:
    const static int CONTROL_PREPARE    = 0;
    const static int CONTROL_NEED_RUN   = 1;
    const static int CONTROL_NEED_PAUSE = 2;
    const static int CONTROL_NEED_EXIT  = 3;

    const static int CONTROL_RUNNING    = 1;
    const static int CONTROL_PAUSE      = 2;
    const static int CONTROL_EXIT       = 3;

private:
    std::map<std::string,std::list<ZSSemaData*>> _subscribers = {};
    std::map<std::string,ZSSemaData*> _databox = {};
    std::thread _t;
    std::string _name;
    SemaphoreO _restart;
};

#endif // __ZSS_PLUGIN_H__
