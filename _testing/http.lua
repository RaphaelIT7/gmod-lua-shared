--[[
	tbl = {
		failed = function(reason) end,
		success = function(body) end,
		method = "GET POST HEAD PUT DELETE PATCH OPTIONS",
		url = "www.google.com",
		body = "",
		type = "text/plain; charset=utf-8",
		timeout = 60,
		headers = {
			["APIKey"] = "SaliBonani" 
		}
	}
]]

function HTTP(tbl)
    local method = tbl.method or "GET"
    local url = tbl.url or ""
    local body = tbl.body or ""
    local contentType = tbl.type or ""
    local headers = ""
    if tbl.headers then
        for key, value in pairs(tbl.headers) do
            headers = headers .. " -H '" .. key .. ": " .. value .. "'"
        end
    end

    local curlCommand = 'curl -s "' .. url .. '"' .. " -H 'Content-Type: application/json' -H 'Authorization: " .. tbl.headers.Authorization .. "' -H 'Accept: application/json'" .. (body == "" and "" or (" --data-raw '" .. body .. "'")) .. " -X " .. method
   	local handle = io.popen(curlCommand)
   	tbl.success(handle:read("*a"))
end

function JSONHTTP(tbl)
	local func = tbl.success
	tbl.success = function(body)
		func(json.decode(body))
	end
	tbl.headers = tbl.headers or {}
	tbl.headers["Accept"] = "application/json"

	HTTP(tbl)
end