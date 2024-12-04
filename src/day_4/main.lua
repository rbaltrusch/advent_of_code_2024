-- read in data as table of tables
local lines = {}
for line in io.lines("src/day_4/data.txt") do
    local line_table = {}
    lines[#lines + 1] = line_table
    local index = 0
    for char in line:gmatch"." do
        index = index + 1
        line_table[index] = char
    end
end

-- part 1: find all occurences of XMAS, horizontal, vertical, diagonal, forwards or backwards
local function create_word_matcher(target_word)
    local length = #target_word
    return function(direction, start_x, start_y)
        local dir_x, dir_y = unpack(direction)
        local string = ""
        for dist = 0, (length - 1) do
            local x = start_x + dir_x * dist
            local y = start_y + dir_y * dist
            local char = (lines[y] or {})[x]
            string = string .. (char or "")
        end
        return string == target_word
    end
end

local directions = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
}

local matcher = create_word_matcher("XMAS")
local count = 0
for start_y, line in ipairs(lines) do
    for start_x = 1, #line do
        for _, direction in ipairs(directions) do
            if matcher(direction, start_x, start_y) then
                count = count + 1
            end
        end
    end
end
print(count)

-- part 2: find two MAS words crossing each other in the shape of an X
matcher = create_word_matcher("MAS")
local reverse_matcher = create_word_matcher("SAM")
local compound_matcher = function(direction, start_x, start_y)
    return matcher(direction, start_x, start_y) or reverse_matcher(direction, start_x, start_y)
end

count = 0
for start_y, line in ipairs(lines) do
    for start_x = 1, #line do
        if compound_matcher({1, 1}, start_x, start_y)
        and compound_matcher({-1, 1}, start_x + 2, start_y)
        then
            count = count + 1
        end
    end
end
print(count)
