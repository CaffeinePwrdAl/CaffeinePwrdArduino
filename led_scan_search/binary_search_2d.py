#
# Copyright (c) 2024 Alex Walters. All rights reserved.
#
# This work is licensed under the terms of the MIT license.  
# For a copy, see <https://opensource.org/licenses/MIT>.
#

#
# Use a set of binary search patterns to search for a lit LED within
# a 2D LED matrix.
#
# Scenario is a pen with a sensor looking at one of the LED's and performing
# an iterative search pattern over the area only knowing at each iteration
# whether the pen saw light or not.
#
# By using a set of patterns, it's possible to exclude all but the pixel
# in question in 8 iterations on a 8x8 LED grid.
#
# Approach should scale to any dimensions and scales with max(width, height)
#

#
# This implementation probably isn't going to win any competitions,
# but it demonstrates the theory.
#

patterns = [
    [ 
        [ 0, 0, 0, 0, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 0, 0, 0, 0 ],
    ],
    [ 
        [ 1, 1, 0, 0, 1, 1, 0, 0 ],
        [ 1, 1, 0, 0, 1, 1, 0, 0 ],
        [ 0, 0, 1, 1, 0, 0, 1, 1 ],
        [ 0, 0, 1, 1, 0, 0, 1, 1 ],
        [ 1, 1, 0, 0, 1, 1, 0, 0 ],
        [ 1, 1, 0, 0, 1, 1, 0, 0 ],
        [ 0, 0, 1, 1, 0, 0, 1, 1 ],
        [ 0, 0, 1, 1, 0, 0, 1, 1 ],
    ],
    [ 
        [ 0, 1, 0, 1, 0, 1, 0, 1 ],
        [ 1, 0, 1, 0, 1, 0, 1, 0 ],
        [ 0, 1, 0, 1, 0, 1, 0, 1 ],
        [ 1, 0, 1, 0, 1, 0, 1, 0 ],
        [ 0, 1, 0, 1, 0, 1, 0, 1 ],
        [ 1, 0, 1, 0, 1, 0, 1, 0 ],
        [ 0, 1, 0, 1, 0, 1, 0, 1 ],
        [ 1, 0, 1, 0, 1, 0, 1, 0 ],
    ],
    [ 
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
    ],
    [ 
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
    ],
    [ 
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 0, 0, 0, 0, 0 ],
    ],
]

def is_set(x, y, pattern_id):
    return patterns[pattern_id][y][x] == 1

def remove_candidates(mask, pattern_id, is_set):
    i = 0
    is_set = 1 if is_set else 0
    print(mask)
    print("---", end=None)
    for y in range(0, 8):
        for x in range(0, 8):
            if patterns[pattern_id][y][x] != is_set:
                idx = x + y * 8
                #print(idx, end=None)            
                mask = mask - set([idx])
    print("---")
    print(mask)
    return mask
    

def find_pixel(x, y):
    valid_spaces = set(x for x in range(0, 64))
    print(valid_spaces)

    for p in range(0, len(patterns)):
        # Subtract pixel locations from our set of valid spaces
        v = remove_candidates(valid_spaces, p, is_set(x, y, p))

        if len(v) != len(valid_spaces):
            print("Pattern", p, "removed", len(valid_spaces) - len(v), "candidates")
            #print(v)
        valid_spaces = v

        # If there's just one space left, search is complete
        if len(valid_spaces) == 1:
            idx = [x for x in valid_spaces][0]
            print("Found pixel at index:", idx)
            print("Iterations = ", p)
            xx = idx % 8
            yy = idx // 8
            return (xx, yy)
    
    print("Pattern still contains", len(valid_spaces), "spaces")
    return (-1, -1)

found = 0
for y in range(0, 8):
    for x in range(0, 8):
        (xx,yy) = find_pixel(x, y)
        if xx != x or yy != y:
            print("WRONGO!")
        if xx == x and yy == y:
            print("Hoorah!", xx, yy)
            found += 1
            
print("Found:", found)
