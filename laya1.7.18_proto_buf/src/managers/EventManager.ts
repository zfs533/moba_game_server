/*
* name;
*/
class EventManager
{
    private static _inst:EventManager = null;
    public static get Instance():EventManager
    {
        return this._inst || (this._inst = new EventManager());
    }
    private event:Array<any>;//{key:string,func_list:[]}
    private packet_event:Array<any>;//{key:number,func_list}
    constructor()
    {
        this.event = new Array<any>();
        this.packet_event = new Array<any>();
    }

    public add_event_listener(event_key:string,callback:Function):void
    {
        for(let i = 0; i< this.event.length; i++)
        {
            if(this.event[i].key == event_key)
            {
                this.event[i].func_list.push(callback);
                return;
            }
        }
        let event_item = {key:event_key,func_list:[callback]};
        this.event.push(event_item);
    }

    public remove_event_listener(event_key:string,callback:Function)
    {
        for(let i = 0; i< this.event.length; i++)
        {
            if(this.event[i].key == event_key)
            {
                for(let j = 0; j< this.event[i].func_list.length; j++)
                {
                    if( this.event[i].func_list[j] == callback)
                    {
                         this.event[i].func_list.splice(j,1);
                         break;
                    }
                }
                if(this.event[i].func_list.length<=0)
                {
                    this.event.splice(i,1);
                }
            }
        }
    }

    public dispatch_event(event_key:string,data:any)
    {
        for(let i = 0; i< this.event.length; i++)
        {
            if(this.event[i].key == event_key)
            {
                for(let j = 0; j< this.event[i].func_list.length; j++)
                {
                    this.event[i].func_list[j](event_key,data);
                }
            }
        }
    }

    public register_event_listener(event_key:number,callback:Function):void
    {
        for(let i = 0; i< this.packet_event.length; i++)
        {
            if(this.packet_event[i].key == event_key)
            {
                this.packet_event[i].func_list.push(callback);
                return;
            }
        }
        let event_item = {key:event_key,func_list:[callback]};
        this.packet_event.push(event_item);
    }

    public remove_register_listener(event_key:number,callback:Function)
    {
        for(let i = 0; i< this.packet_event.length; i++)
        {
            if(this.packet_event[i].key == event_key)
            {
                for(let j = 0; j< this.packet_event[i].func_list.length; j++)
                {
                    if( this.packet_event[i].func_list[j] == callback)
                    {
                         this.packet_event[i].func_list.splice(j,1);
                         break;
                    }
                }
                if(this.packet_event[i].func_list.length<=0)
                {
                    this.packet_event.splice(i,1);
                }
            }
        }
    }

    public dispatch_register_event(event_key:number,data:Uint8Array)
    {
        for(let i = 0; i< this.packet_event.length; i++)
        {
            if(this.packet_event[i].key == event_key)
            {
                for(let j = 0; j< this.packet_event[i].func_list.length; j++)
                {
                    this.packet_event[i].func_list[j](event_key,data);
                }
            }
        }
    }

}