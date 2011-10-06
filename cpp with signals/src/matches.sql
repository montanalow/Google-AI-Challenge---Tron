table matches (
 integer player_one_entry_id,
 integer player_two_entry_id,
 integer winner_id nullable, -- null winner_id indicates a draw
 integer map_id
);

table entries (
  integer id,
  integer games_played, -- the total number of games played
  integer rating -- newcomers start with a rating of 1500?
);


while true {
  -- select the challenger as the person with the fewest games played
  challenger =
    select entries.*
    from entries
    group by entries.id
    order by games_played ASC
    limit 1

  -- select the oponent that is ranked directly above the challenger, that has played the fewest number of games if there are multiple entries with the same rating
  opponent =
    select entries.*
    from entries
    where rating > #{challenger.rating}
    order by rating ASC, games_played ASC
    limit 1

  -- select the map that has been played the fewests times by both the challenger and opponent
  map =
    select maps.id, count( matches.map_id ) as times_played
    from maps
    left outer join matches
      on player_one_entry_id = #{challenger.id}
      or player_two_entry_id = #{challenger.id}
      or player_one_entry_id = #{opponent.id}
      or player_two_entry_id = #{opponent.id}
    group by maps.id
    order by times_played ASC

  -- play the game
  winner_id = play_game( challenger, opponent, map );

  -- record the results
  insert into matches( challenger_id, opponent_id, winner_id, map_id );
  update entries set games_played = games_played + 1 where id = #{challenger_id} or id = #{opponent.id}

  -- update the ratings elo-esquely using the same k-factors as the USCF
  challenger_score = 0
  opponent_score = 0
  if( winner_id = challenger.id ) {
    challenger_score = 1
  } else if( winner_id = opponent.id ) {
    opponent_score = 1
  } else {
    challenger_score = 0.5
    opponent_score = 0.5
  }

  k = 32
  if( challenger.rating > 2100 ) {
    k = 24
  } else if( challenger.rating > 2400 ) {
    k = 16
  }

  qa = 10 ^ (challenger.rating / 400)
  qb = 10 ^ (opponent.rating / 400)
  expected_challenger_score = qa / qa + qb
  expected_opponent_score = qb / qa + qb

  update entries set rating = rating + #{k} * ( #{challenger_score} - #{expected_challenger_score} ) where id = #{challenger.id}
  update entries set rating = rating + #{k} * ( #{opponent_score} - #{expected_opponent_score} ) where id = #{opponent.id}
}
