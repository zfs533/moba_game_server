/*
* name;
*/
class EventManager {
    constructor() {
        this.event = new Array();
        this.packet_event = new Array();
    }
    static get Instance() {
        return this._inst || (this._inst = new EventManager());
    }
    add_event_listener(event_key, callback) {
        for (let i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                this.event[i].func_list.push(callback);
                return;
            }
        }
        let event_item = { key: event_key, func_list: [callback] };
        this.event.push(event_item);
    }
    remove_event_listener(event_key, callback) {
        for (let i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                for (let j = 0; j < this.event[i].func_list.length; j++) {
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
    }
    dispatch_event(event_key, data) {
        for (let i = 0; i < this.event.length; i++) {
            if (this.event[i].key == event_key) {
                for (let j = 0; j < this.event[i].func_list.length; j++) {
                    this.event[i].func_list[j](event_key, data);
                }
            }
        }
    }
    register_event_listener(event_key, callback) {
        for (let i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                this.packet_event[i].func_list.push(callback);
                return;
            }
        }
        let event_item = { key: event_key, func_list: [callback] };
        this.packet_event.push(event_item);
    }
    remove_register_listener(event_key, callback) {
        for (let i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                for (let j = 0; j < this.packet_event[i].func_list.length; j++) {
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
    }
    dispatch_register_event(event_key, data) {
        for (let i = 0; i < this.packet_event.length; i++) {
            if (this.packet_event[i].key == event_key) {
                for (let j = 0; j < this.packet_event[i].func_list.length; j++) {
                    this.packet_event[i].func_list[j](event_key, data);
                }
            }
        }
    }
}
EventManager._inst = null;
//# sourceMappingURL=EventManager.js.map