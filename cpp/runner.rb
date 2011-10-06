maps = Dir.entries "./maps"
bots = Dir.entries "./example_bots"

maps = maps.select{|m| m != '.' && m != '..' }
bots = bots.select{|b| b != '.' && b != '..' && !b.include?("zug_zug") && !b.include?("tug_tug")}

wins = 0
losses = 0
collisions = 0
draws = 0
m_results = Hash.new {|hash,key| hash[key] = {:wins => 0, :losses => 0, :draws => 0, :collisions => 0} }
maps.each do |m|
  bots.each do |b|
    next if m == "huge-room.txt" || m.include?("test") || m.include?( "profile" )
#    debug = `java -jar engine/Tron.jar maps/#{m} "./build/zug_zug" "java -jar ./example_bots/#{b}" 0 1`
    debug = `java -jar engine/Tron.jar maps/#{m} "./build/zug_zug" "java -jar ./example_bots/#{b}" 0 1`
    result = debug.split("\n").last
    if result == "Player One Wins!"
      m_results[m][:wins] += 1
      wins += 1
    elsif result == "Players collided. Draw!"
      m_results[m][:draws] += 1
      draws += 1
    elsif result == "Player Two Wins!"
      m_results[m][:losses] += 1
      losses += 1
      puts debug
    else
      m_results[m][:collisions] += 1
      collisions += 1
    end
    puts "#{b} @ #{m}: #{result.split("\n").last}"

    debug = `java -jar engine/Tron.jar maps/#{m} "java -jar ./example_bots/#{b}" "./build/zug_zug" 0 1`
    result = debug.split("\n").last
    if result == "Player Two Wins!"
      m_results[m][:wins] += 1
      wins += 1
    elsif result == "Players collided. Draw!"
      m_results[m][:draws] += 1
      draws += 1
    elsif result == "Player One Wins!"
      m_results[m][:losses] += 1
      losses += 1
      puts debug
    else
      m_results[m][:collisions] += 1
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
m_results.each do |map, results|
  puts "================================"
  puts "@ #{map}"
  puts "Wins: #{results[:wins]}"
  puts "Losses: #{results[:losses]}"
  puts "Draws: #{results[:draws]}"
  puts "Collisions: #{results[:collisions]}"
  puts "-------------------------------"
  puts "Ratio: #{(results[:wins].to_f + 0.5 * (results[:draws] + results[:collisions])) / (results[:wins] + results[:losses] + results[:draws] + results[:collisions])}"
end
