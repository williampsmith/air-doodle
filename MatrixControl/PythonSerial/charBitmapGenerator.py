from __future__ import print_function
import string
import os
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import numpy as np

def char_to_pixels(text, path, fontsize=14):
    """
    Based on http://stackoverflow.com/a/27753869/190597 (jsheperd)
    """
    font = ImageFont.truetype(path, fontsize) 
    w, h = font.getsize(text)  
    h *= 2
    image = Image.new('L', (w, h), 1)   #create a b/w image
    draw = ImageDraw.Draw(image)
    draw.text((0, 0), text, font=font) 

    arr = np.asarray(image)
    arr = np.where(arr, 0, 1)
    arr = arr[(arr != 0).any(axis=1)]
    return arr

def display(arr):
    result = np.where(arr, '1', ' ')
    print('\n'.join([''.join(row) for row in result]))

for c in ['5', '6', '7', '8']:
    FONT_PATH = os.environ.get("FONT_PATH", "/Library/Fonts/AppleGothic.ttf")
    arr = char_to_pixels(c, FONT_PATH, fontsize=15)
    print(arr.shape)
    display(arr)
    print(arr)
    print()
