if (!('indexOf' in Array.prototype)) {
    Array.prototype.indexOf= function(find, i /*opt*/) {
        if (i===undefined){
            i= 0;
        }
        if (i<0){
            i+= this.length;
        }
        if (i<0){
            i= 0;
        }
        for (var n= this.length; i<n; i++){
            if (i in this && this[i]===find){
                return i;
            }
        }
        return -1;
};
}

Array.prototype.unique = function()
{
    var n = {},r=[];
    for(var i = 0; i < this.length; i++) 
    {
        if (!n[this[i]]) 
        {
            n[this[i]] = true; 
            r.push(this[i]); 
        }
    }
    return r;
};

function Test(){
    var test = 'test';
    try {
        JSON.parse("{}");
        localStorage.setItem(test, test);
        localStorage.removeItem(test);
        return true;
    } catch(e) {
        return false;
    }
};
function FixedQueue(size){
    var queue = new Array();
    queue.fixedSize = size;
    queue.add = FixedQueue.add;
    queue.load = FixedQueue.load;
    queue.get = FixedQueue.get;
    return( queue );
};
FixedQueue.add = function(arg1,arg2){
    arg2 = (arg2 || false);
    if (arg2)
    {
        if (arg1 < this.fixedSize)
        {
            this[arg1] = arg2;
        }
    }
    else
    {
        if (this.indexOf(arg1) < 0)
        {
            this.push(arg1);
        }
    }
    
    if (this.length <= this.fixedSize){
        return;
    }
    Array.prototype.splice.call(
        this,
        0,
        (this.length - this.fixedSize)
    );

};
FixedQueue.load = function(arg1,arg2){
    arg2 = (arg2 || false);
    if (arg2)
    {
        if (arg1 < this.fixedSize)
        {
            this[arg1] = arg2;
        }
    }
    else
    {
        if (this.indexOf(arg1) < 0)
        {
            this.push(arg1);
        }
    }
    
    if (this.length <= this.fixedSize){
        return;
    }
    Array.prototype.splice.call(
        this,
        0,
        (this.length - this.fixedSize)
    );

};
FixedQueue.get = function(){
    Array.prototype.splice.call(
        this,
        0,
        (this.length - this.fixedSize)
    );
    return this.join(";");
};

function ExcludeOffers(invert,counter){
    var invert_ = Boolean(invert || false);
    var counter_ = Boolean(counter || false);
    initialValues = new Object({});
    initialValues.add = ExcludeOffers.add;
    initialValues.load = ExcludeOffers.load;
    initialValues.getString = ExcludeOffers.getString;
    initialValues.get = ExcludeOffers.get;
    initialValues.invert = function(){return invert_};
    initialValues.counter = function(){return counter_};
    return(initialValues);
};
ExcludeOffers.add = function(guid, countViews){
    countViews = (countViews || 1);
    if (typeof(this[guid]) == 'number')
    {
        if (this.invert())
        {
            this[guid] = ++this[guid];
        }
        else
        {
            if (this[guid] > 0)
            {
                this[guid] = --this[guid];
            }
            else
            {
                this[guid] = 0;
            }
        }
    }
    else
    {
        if (this.invert())
        {
            this[guid] = countViews;
        }
        else
        {
            this[guid] = countViews - 1 ;
        }
    }
};

ExcludeOffers.load = function(guid, countViews){
    this[guid] = countViews;
};

ExcludeOffers.getString = function(){
    keys = [];
    if(this.counter())
    {
        for(var key in this) {
            var value = this[key];
            if(this.invert())
            {
                if (value > 0)
                {
                    keys.push(key +"~"+value);
                }
            }
            else
            {
                if (value <= 0)
                {
                    keys.push(key +"~"+value);
                }
            }
        }
    }
    else
    {
        for(var key in this) {
            var value = this[key];
            if(this.invert())
            {
                if (value > 0)
                {
                    keys.push(key);
                }
            }
            else
            {
                if (value <= 0)
                {
                    keys.push(key);
                }
            }
        }
    }
    return keys.join(";");
};

ExcludeOffers.get = function(){
    keys = [];
    if(this.counter())
    {
        for(var key in this) {
            var value = this[key];
            if(this.invert())
            {
                if (value > 0)
                {
                    keys.push([key, value]);
                }
            }
            else
            {
                if (value <= 0)
                {
                    keys.push([key,value]);
                }
            }
        }
    }
    else
    {
        for(var key in this) {
            var value = this[key];
            if(this.invert())
            {
                if (value > 0)
                {
                    keys.push(key);
                }
            }
            else
            {
                if (value <= 0)
                {
                    keys.push(key);
                }
            }
        }
    }
    return keys;
};

function RetargetingOffers(){
    initialValues = new Object({});
    initialValues.add = RetargetingOffers.add;
    initialValues.load = RetargetingOffers.load;
    initialValues.get = RetargetingOffers.get;
    initialValues.remove = RetargetingOffers.remove;
    return(initialValues);
};

RetargetingOffers.add = function(guid, arg1, arg2, arg3, arg4){
    this[guid] = [arg1, arg2, arg3, arg4];
};

RetargetingOffers.load = function(guid, arg1){
    if( Object.prototype.toString.call( arg1 ) === '[object Array]' ) {
        if (typeof arg1[3] == 'undefined')
        {
            arg1[3] = arg1[0]
        }
        this[guid] = [arg1[0], arg1[1], arg1[2], arg1[3]];
    }
};


RetargetingOffers.remove = function(key){
    delete this[key]
};

RetargetingOffers.get = function(){
    keys = [];
    res = [];
    time = Math.floor(Date.now());
    for(var key in this) {
        var value = this[key];
        if (value[0] > time)
        {
            keys.push([
            key.split('...')[0],
            this[key][2],
            this[key][1],
            parseInt(this[key][3]),
            Math.abs(new Date(Math.floor(Date.now())).getDate() - new Date(this[key][3] * 1000).getDate())
            ]);
        }
    }
    keys.sort(function(a,b){
        if (a[3]<b[3])
        {
            return -1;
        }
        if (a[3]>b[3])
        {
            return 1;
        }
        return 0;
    }
    );
    for(x in keys) {
        if (typeof(keys[x]) != 'function')
        {
            res.push([keys[x][0]+"~"+keys[x][1]+"~"+keys[x][2]+"~"+keys[x][4]]);
        }
    }
    return res.join(";");
};

function GenderAccount(){
    initialValues = new Object({});
    initialValues.add = GenderAccount.add;
    initialValues.get = GenderAccount.get;
    initialValues.load = GenderAccount.load;
    return(initialValues);
};

GenderAccount.add = function(guid, val)
{
    if (typeof this[guid] == 'undefined')
    {
        var hit_log = new Array(0,0,0);
        hit_log[val] += 1;
        this[guid] = [val, hit_log]
    }
    else
    {
        var hit_log = this[guid][1];
        hit_log[val] += 1;
        hit_log[0] = 1;
        this[guid] = [hit_log.indexOf(Math.max.apply(Math, hit_log)), hit_log];
    }
};
GenderAccount.get = function()
{
    res = [];
    for(var key in this) {
        if (typeof this[key][0] != 'undefined')
        {
            var value = this[key][0];
            res.push([key+"~"+value]);
        }
    }
    return res.join(";");
};
GenderAccount.load = function(guid, arg1){
    if( Object.prototype.toString.call( arg1 ) === '[object Array]' ) {
        this[guid] = [arg1[0], arg1[1]];
    }
};
function GenderUser(){
    initialValues = new Object({});
    initialValues.add = GenderUser.add;
    initialValues.get = GenderUser.get;
    initialValues.load = GenderUser.load;
    return(initialValues);
};
GenderUser.add = function(val)
{
    if (typeof this['gender'] == 'undefined')
    {
        this['gender'] = val;
        var hit_log = new Array(0,0,0);
        hit_log[val] += 1;
        this['hit_log'] = hit_log;
    }
    else
    {
        if( Object.prototype.toString.call( this['hit_log'] ) === '[object Array]' ) {
            var hit_log = this['hit_log'];
            hit_log[val] += 1;
            hit_log[0] = 1;
            this['hit_log'] = hit_log;
            this['gender'] = hit_log.indexOf(Math.max.apply(Math, hit_log));
        }
    }
};
GenderUser.get = function()
{
    var res = '';
    if (typeof this['gender'] != 'undefined')
    {
        res = this['gender'];
    }
    return res;
};
GenderUser.load = function(guid, arg1){
    this[guid] = arg1;
};

function CostAccount(){
    initialValues = new Object({});
    initialValues.add = CostAccount.add;
    initialValues.get = CostAccount.get;
    initialValues.load = CostAccount.load;
    return(initialValues);
};
CostAccount.add = function(guid, val)
{
    if (typeof this[guid] == 'undefined')
    {
        var hit_log = new Array(0,0,0,0,0,0,0,0,0);
        hit_log[val] += 1;
        this[guid] = [val, hit_log]
    }
    else
    {
        var hit_log = this[guid][1];
        hit_log[val] += 1;
        hit_log[0] = 1;
        this[guid] = [hit_log.indexOf(Math.max.apply(Math, hit_log)), hit_log];
    }
};
CostAccount.get = function()
{
    res = [];
    for(var key in this) {
        if (typeof this[key][0] != 'undefined')
        {
            var value = this[key][0];
            res.push([key+"~"+value]);
        }
    }
    return res.join(";");
};
CostAccount.load = function(guid, arg1){
    if( Object.prototype.toString.call( arg1 ) === '[object Array]' ) {
        this[guid] = [arg1[0], arg1[1]];
    }
};

function CostUser(){
    initialValues = new Object({});
    initialValues.add = CostUser.add;
    initialValues.get = CostUser.get;
    initialValues.load = CostUser.load;
    return(initialValues);
};
CostUser.add = function(val)
{
    if (typeof this['cost'] == 'undefined')
    {
        this['cost'] = val;
        var hit_log = new Array(0,0,0,0,0,0,0,0,0);
        hit_log[val] += 1;
        this['hit_log'] = hit_log;
    }
    else
    {
        if( Object.prototype.toString.call( this['hit_log'] ) === '[object Array]' ) {
            var hit_log = this['hit_log'];
            hit_log[val] += 1;
            hit_log[0] = 1;
            this['hit_log'] = hit_log;
            this['cost'] = hit_log.indexOf(Math.max.apply(Math, hit_log));
        }
    }
};
CostUser.get = function()
{
    var res = '';
    if (typeof this['cost'] != 'undefined')
    {
        res = this['cost'];
    }
    return res;
};
CostUser.load = function(guid, arg1){
    this[guid] = arg1;
};
function ActivityAccount(){
    initialValues = new Object({});
    initialValues.add = ActivityAccount.add;
    initialValues.load = ActivityAccount.load;
    return(initialValues);
};
ActivityAccount.add = function(guid, timeFirst, timeLast){
    if (typeof this[guid] == 'undefined')
    {
        this[guid] = [timeFirst, timeLast];
    }
    else
    {
        this[guid][1] = timeLast;
    }
};
ActivityAccount.load = function(guid, arg1){
    if( Object.prototype.toString.call( arg1 ) === '[object Array]' ) {
        this[guid] = [arg1[0], arg1[1]];
    }
};

function ActivityUser(){
    initialValues = new Object({});
    initialValues.add = ActivityUser.add;
    initialValues.load = ActivityUser.load;
    return(initialValues);
};
ActivityUser.add = function(timeFirst, timeLast){
    if (typeof this['timeFirst'] == 'undefined')
    {
        this['timeFirst'] = timeFirst;
    }
    this['timeLast'] = timeLast;
};
ActivityUser.load = function(guid, arg1){
    this[guid] = arg1;
};

function UserHistory(){
	this.searchengines = new FixedQueue(3);
	this.context = new FixedQueue(3);
	this.retargeting = new RetargetingOffers();
	this.exclude = new ExcludeOffers();
	this.exclude_click = new ExcludeOffers();
	this.retargeting_exclude = new ExcludeOffers();
	this.retargeting_account_exclude = new ExcludeOffers();
	this.retargeting_exclude_click = new ExcludeOffers();
	this.retargeting_account_exclude_click = new ExcludeOffers();
	this.retargeting_view = new ExcludeOffers(true,true);
	this.history = new FixedQueue(3);
    this.load = UserHistory.load;
    this.save = UserHistory.save;
    this.exclude_clean = UserHistory.exclude_clean;
    this.exclude_click_clean = UserHistory.exclude_click_clean;
    this.exclude_get = UserHistory.exclude_get;
    this.retargeting_clean = UserHistory.retargeting_clean;
    this.retargeting_account_clean = UserHistory.retargeting_account_clean;
    this.retargeting_click_clean = UserHistory.retargeting_click_clean;
    this.retargeting_exclude_get = UserHistory.retargeting_exclude_get;
    this.retargeting_account_exclude_get = UserHistory.retargeting_account_exclude_get;
    this.gender_accounts = new GenderAccount();
    this.gender_user = new GenderUser();
    this.cost_accounts = new CostAccount();
    this.cost_user = new CostUser();
    this.activity_accounts = new ActivityAccount();
    this.activity_user = new ActivityUser();
};

UserHistory.load = function(){
    if (Test() === true)
    {
        for (key in this)
        {
            if (typeof(this[key]) != 'function')
            {
                var history_name = key;
                var retrievedObject = JSON.parse(localStorage.getItem(history_name));
                for ( key in retrievedObject)
                {   
                    this[history_name].load(key, retrievedObject[key]);
                }
            }
        }
        return true;
    }
    return false;
};
UserHistory.save = function(){
    if (Test() === true)
    {
        for (key in this)
        {
            if (typeof(this[key]) != 'function')
            {
                localStorage.setItem(key, JSON.stringify(this[key]));
            }
        }
        return true;
    }
    return false;
};

UserHistory.exclude_clean = function(cl){
    if(cl)
    {
        this.exclude = new ExcludeOffers();
        this.save();
    }
    return cl;
};
UserHistory.exclude_click_clean = function(cl){
    if(cl)
    {
        this.exclude_click = new ExcludeOffers();
        this.save();
    }
    return cl;
};
UserHistory.exclude_get = function(){
    var keys = [];
    keys = this.exclude.get().concat(this.exclude_click.get());
    keys = keys.unique();
    return keys.join(';') ;
};
UserHistory.retargeting_clean = function(cl){
    if(cl)
    {
        this.retargeting_exclude = new ExcludeOffers();
        this.retargeting_view = new ExcludeOffers(true,true);
        this.save();
    }
    return cl;
};
UserHistory.retargeting_account_clean = function(cl){
    if(cl)
    {
        this.retargeting_account_exclude = new ExcludeOffers();
        this.save();
    }
    return cl;
};
UserHistory.retargeting_click_clean = function(cl){
    if(cl)
    {
        this.retargeting_click_exclude = new ExcludeOffers();
        this.save();
    }
    return cl;
};
UserHistory.retargeting_exclude_get = function(){
    var keys = [];
    keys = this.retargeting_exclude.get().concat(this.retargeting_exclude_click.get());
    keys = keys.unique();
    return keys.join(';') ;
};
UserHistory.retargeting_account_exclude_get = function(){
    var keys = [];
    keys = this.retargeting_account_exclude.get().concat(this.retargeting_account_exclude_click.get());
    keys = keys.unique();
    return keys.join(';') ;
};
