json = require("json")
require("http")

local args = {...}
local url = args[1]
local id = args[2]
local api = args[3]

local header = {
	["Accept"] = "application/json",
	["Content-Type"] = "application/json",
	["Authorization"] = "Bearer " .. api,
}

HTTP({
	method = "POST",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/delete",
	headers = header,
	success = function(res)
		local tbl = res ~= '' and json.decode(res) or {}
		if tbl.errors then
			print("debug.log failed! Reason: " .. tbl.errors[1].detail)
		end
	end,
	body = json.encode({
		root = "/",
		files = {
			"debug.log"
		}
	})
})

HTTP({
	method = "POST",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/delete",
	headers = header,
	success = function(res)
		local tbl = res ~= '' and json.decode(res) or {}
		if tbl.errors then
			print("console.log failed! Reason: " .. tbl.errors[1].detail)
		end
	end,
	body = json.encode({
		root = "/garrysmod/",
		files = {
			"console.log"
		}
	})
})

HTTP({
	method = "POST",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/delete",
	headers = header,
	success = function(res)
		local tbl = res ~= '' and json.decode(res) or {}
		if tbl.errors then
			print("error.txt failed! Reason: " .. tbl.errors[1].detail)
		end
	end,
	body = json.encode({
		root = "/garrysmod/data/",
		files = {
			"error.txt"
		}
	})
})

JSONHTTP({
	method = "GET",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/resources",
	headers = header,
	success = function(content)
		if content.attributes and content.attributes.current_state == "running" then
			HTTP({
				method = "POST",
				url = "https://" .. url .. "/api/client/servers/" .. id .. "/command",
				headers = header,
				success = function(res)
					local tbl = res ~= '' and json.decode(res) or {}
					if tbl.errors then
						print("Commands failed! Reason: " .. tbl.errors[1].detail)
					end
				end,
				body = json.encode({
					command = "quit"
				})
			})

			print("Waiting for Server to stop")

			local stopped = false
			while not stopped do
				JSONHTTP({
					method = "GET",
					url = "https://" .. url .. "/api/client/servers/" .. id .. "/resources",
					headers = header,
					success = function(content)
						if content.attributes and content.attributes.current_state == "offline" then
							stopped = true
						end
					end,
				})
			end

			print("Server stopped")
		end
	end,
})

HTTP({
	method = "POST",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/power",
	headers = header,
	success = function(res)
		local tbl = res ~= '' and json.decode(res) or {}
		if tbl.errors then
			print("Restart failed! Reason: " .. tbl.errors[1].detail)
		else
			print("Restarted successfully")
		end
	end,
	body = json.encode({
		signal = "start"
	})
})

print("Waiting for Server to start")

--[[local started = false
while not started do
	JSONHTTP({
		method = "GET",
		url = "https://" .. url .. "/api/client/servers/" .. id .. "/resources",
		headers = header,
		success = function(content)
			if content.attributes and content.attributes.current_state == "running" then
				started = true
			end
		end,
	})
end

print("Server started")]]

local function Sleep(time)
	local time = os.time() + time
	while time > os.time() do end
end

Sleep(30)

HTTP({
	method = "POST",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/command",
	headers = header,
	success = function(res)
		local tbl = res ~= '' and json.decode(res) or {}
		if tbl.errors then
			print("Commands failed! Reason: " .. tbl.errors[1].detail)
		end
	end,
	body = json.encode({
		command = "quit"
	})
})

print("Waiting for Server to stop")

local stopped = false
while not stopped do
	JSONHTTP({
		method = "GET",
		url = "https://" .. url .. "/api/client/servers/" .. id .. "/resources",
		headers = header,
		success = function(content)
			if content.attributes and content.attributes.current_state == "offline" then
				stopped = true
			end
		end,
	})
end

print("Server stopped")

local STATUS = {
	CRASH = 1,
	ERROR = 2,
	WORKS = 3
}

local Status = STATUS.WORKS
HTTP({
	method = "GET",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/contents?file=%2Fdebug.log",
	headers = header,
	success = function(content)
		if not content:find([[{"errors":]]) then
			print(" ======================= debug.log =======================")
			print(content)
			print(" ======================= debug.log =======================")
			Status = STATUS.CRASH
			--error("Server crashed!")
		else
			--print("debug.log not found?", json.decode(content).errors[1].detail)
		end
	end,
})

if STATUS.WORKS then
	HTTP({
		method = "GET",
		url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/contents?file=%2Fgarrysmod%2Fdata%2Ferror.txt",
		headers = header,
		success = function(content)
			if not content:find([[{"errors":]]) then
				print(" ======================= error.txt =======================")
				print(content)
				print(" ======================= error.txt =======================")
				Status = STATUS.ERROR
				--error("Failed to load Module or an error ocurred!")
			else
				--print("error.txt not found?", json.decode(content).errors[1].detail)
			end
		end,
	})
end

HTTP({
	method = "GET",
	url = "https://" .. url .. "/api/client/servers/" .. id .. "/files/contents?file=%2Fgarrysmod%2Fconsole.log",
	headers = header,
	success = function(content)
		if not content:find([[{"errors":]]) then
			print(" ======================= console.log =======================")
			print(content)
			print(" ======================= console.log =======================")
		else
			--print("console.log not found?", json.decode(content).errors[1].detail)
		end
	end,
})

if Status ~= STATUS.WORKS then
	Sleep(3)
	print("Error")
	error("Something broke!")
end