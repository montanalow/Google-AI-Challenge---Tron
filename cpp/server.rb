while true
  puts `./tcptron 213.3.30.106 9999 montanalow ./build/zug_zug`
  (1..5).each do |i|
    puts "#{i} press ctrl-c to abort next game"
    sleep 1
  end
end