-- Initializations of equipment
do

    print("Lua Execution Started")

end

while true do

    print("Executing the scripts...")

    local n = 5
    print("Factorial of "..n.." : "..factorial(n))

    Grb_collect() -- Garbage collector

    delay(2000) -- delay of 2 seconds
end