local POOL_SIZE = 20000

local ffi = require("ffi")
local type = type
local table = table

ffi.cdef [[
typedef struct { float x, y, z; } GMOD_Vector_t;
]]

function isvector(v)
    return ffi.istype("GMOD_Vector_t", v)
end

local methods = {}
local mt = {
    __index = function(s, k)
        if type(k) == "number" then
            if k == 1 then
                return s.x
            elseif k == 2 then
                return s.y
            elseif k == 3 then
                return s.z
            end
        end
        return methods[k]
    end,
    __eq = function(a, b)
        if not isvector(b) then return false end
        return a.x == b.x and a.y == b.y and a.z == b.z
    end,
    __add = function(a, b) return Vector(a.x + b.x, a.y + b.y, a.z + b.z) end,
    __sub = function(a, b) return Vector(a.x - b.x, a.y - b.y, a.z - b.z) end,
    __mul = function(a, b) return Vector(a.x * b.x, a.y * b.y, a.z * b.z) end,
    __div = function(a, b) return Vector(a.x / b.x, a.y / b.y, a.z / b.z) end,
    __unm = function(a) return Vector(-a.x, -a.y, -a.z) end,
    __tostring = function(a)
        return string.format("%f %f %f", a.x, a.y, a.z)
    end
}

function methods:Add(v)
    self.x, self.y, self.z = self.x + v.x, self.y + v.y, self.z + v.z
end

local RawVector = ffi.metatype("GMOD_Vector_t", mt)

local POOL = {}
local function add_to_pool(v)
    table.insert(POOL, v)
    ffi.gc(v, add_to_pool)
end
for i = 1, POOL_SIZE do
    local v = RawVector()
    add_to_pool(v)
end

function Vector(x, y, z)
    if x == nil then
        x = 0
    end
    if y == nil then
        y = 0
    end
    if z == nil then
        z = 0
    end
    local v_pool = table.remove(POOL)
    if v_pool then
        v_pool.x, v_pool.y, v_pool.z = x, y, z
        return v_pool
    end
    return RawVector(x, y, z)
end

local v = Vector(1, 2, 3)
print(#POOL)
v = nil

collectgarbage()
print(#POOL)

local v = Vector(1, 2, 3)
print(#POOL)
v = nil

collectgarbage()
print(#POOL)