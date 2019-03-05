function disconnected(data)
  logger.debug("disconnect------------")
  local port ,ip = session.get_address(data)
  logger.debug(port,ip)
end

function recv_cmd_func(s,msg)
    logger.debug(msg[1])
    logger.debug(msg[2])
    logger.debug(msg[3])

    local body = msg[4]
    for k,v in pairs(body) do
      logger.debug(k,v)
    end
    --send data to client
    local client_data = {1,2,0,{status = 200}}-- same as protobuf map data
    session.send_msg(s,client_data)

end

local abc = {
  on_session_recv_cmd = recv_cmd_func,
  on_session_disconnect = disconnected,
}

local service_cmd = {
  stype = 1,
  service = abc, 
}

return service_cmd