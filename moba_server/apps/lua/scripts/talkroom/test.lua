print("hello test.lua script")

local key = ""
function PrintTable(table , level)
  level = level or 1
  local indent = ""
  for i = 1, level do
    indent = indent.."  "
  end

  if key ~= "" then
    print(indent..key.." ".."=".." ".."{")
  else
    print(indent .. "{")
  end

  key = ""
  for k,v in pairs(table) do
     if type(v) == "table" then
        key = k
        PrintTable(v, level + 1)
     else
        local content = string.format("%s%s = %s", indent .. "  ",tostring(k), tostring(v))
      print(content)  
      end
  end
  print(indent .. "}")

end

mysql_wrapper.connect("127.0.0.1",3306,"my_test","root","123456",function(err,context)
	if err then 
		print("err---connect")
	else
		mysql_wrapper.query(context,"select * from student",function(err,result)
			print(result)
			PrintTable(result)
		end)
	end
print("callback_func")
end);
local mm = {
	a = 1,
	b = 2,
	c = 3,
}

local timer = scheduler.scheduler(function() 
  logger.debug("scheduler.scheduler------------")
end,5000,1000,-1)

scheduler.once(function()
  scheduler.cancel(timer)
end,8000)

return mm;