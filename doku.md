Bewertungsfunktion: -> material (player perspective) 
                    -> material + position (player perspective) 
                    -> material + position + control (player perspective) 
                    -> material + position + control + odd_even : positive ratings for white negative for black

Alpha-Beta:         ->  
                    -> 
                    -> 
                    -> 

Hash-Function:      ->  
                    -> 
                    -> 
                    -> 

Trans Table:        ->  
                    -> 
                    -> 
                    -> 

Bitboards:          ->  
                    -> 
                    -> 
                    -> 

Move_generator:     ->  
                    -> 
                    -> 
                    -> 

Dynamic time Management:
chess clock: total_remaining_time (common.c) = default_time_per_move * default_expected_move_num
dekremented: after move done
exception: if depth is almost done: letting depth finish
time budged for next move: time budged based on num of first depth child nodes in relation to size of remaining chess clock
equation: y = 2.437762 + (0.2002329 - 2.437762)/(1 + (x/31.05609)^5.557229)
sampled by curve fitting of dataset:

         x                 y
         20                0.4        
         50                2.3        
         40                2          
         30                1.2        
         60                2.4        
         80                2.4        
         10                0.2        
          5                0.19       