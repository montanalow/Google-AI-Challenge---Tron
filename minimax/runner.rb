maps = Dir.entries "./maps"
bots = Dir.entries "./example_bots"

maps = maps.select{|m| m != '.' && m != '..' }
bots = bots.select{|m| m != '.' && m != '..' }
wins = 0
losses = 0
collisions = 0
draws = 0
maps.each do |m|
  bots.each do |b|
    next if m == "huge-room.txt" || m.include?("test")
    debug = `java -jar engine/Tron.jar maps/#{m} "./MyTronBot" "java -jar example_bots/#{b}" 0`
    result = debug.split("\n").last
    if result == "Player One Wins!"
      wins += 1
    elsif result == "Players collided. Draw!"
      draws += 1
    elsif result == "Player Two Wins!"
      losses += 1
      puts debug
    else
      collisions += 1
    end
    puts "#{b} @ #{m}: #{result.split("\n").last}"
  end
end

puts "-------------------------------"
puts "Wins: #{wins}"
puts "Losses: #{losses}"
puts "Draws: #{draws}"
puts "Collisions: #{collisions}"
puts "-------------------------------"
puts "Ratio: #{(wins.to_f + 0.5 * (draws + collisions)) / (wins + losses + draws + collisions)}"