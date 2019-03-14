/*
* name;
*/
var EventManager = /** @class */ (function () {
    function EventManager() {
        this.event = new Array();
        this.packet_event = new Array();
    }
    Object.defineProperty(EventManager, "Instance", {
        get: function () {
            return this._inst || (this._inst = new EventManager());
        },
        enumerable: true,
        configurable: true
    });
    EventManager.prototype.add_event_listener = function (event_key, callback) {
        for (var i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                this.event[i].func_list.push(callback);
                return;
            }
        }
        var event_item = { key: event_key, func_list: [callback] };
        this.event.push(event_item);
    };
    EventManager.prototype.remove_event_listener = function (event_key, callback) {
        for (var i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                for (var j = 0; j < this.event[i].func_list.length; j++) {
                    if (this.event[i].func_list[j] == callback) {
                        this.event[i].func_list.splice(j, 1);
                        break;
                    }
                }
                if (this.event[i].func_list.length <= 0) {
                    this.event.splice(i, 1);
                }
            }
        }
    };
    EventManager.prototype.dispatch_event = function (event_key, data) {
        for (var i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                for (var j = 0; j < this.event[i].func_list.length; j++) {
                    this.event[i].func_list[j](event_key, data);
                }
            }
        }
    };
    EventManager.prototype.register_event_listener = function (event_key, callback) {
        for (var i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                this.packet_event[i].func_list.push(callback);
                return;
            }
        }
        var event_item = { key: event_key, func_list: [callback] };
        this.packet_event.push(event_item);
    };
    EventManager.prototype.remove_register_listener = function (event_key, callback) {
        for (var i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                for (var j = 0; j < this.packet_event[i].func_list.length; j++) {
                    if (this.packet_event[i].func_list[j] == callback) {
                        this.packet_event[i].func_list.splice(j, 1);
                        break;
                    }
                }
                if (this.packet_event[i].func_list.length <= 0) {
                    this.packet_event.splice(i, 1);
                }
            }
        }
    };
    EventManager.prototype.dispatch_register_event = function (event_key, data) {
        for (var i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                for (var j = 0; j < this.packet_event[i].func_list.length; j++) {
                    this.packet_event[i].func_list[j](event_key, data);
                }
            }
        }
    };
    EventManager._inst = null;
    return EventManager;
}());
//# sourceMappingURL=EventManager.js.map