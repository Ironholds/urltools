s = """\
import urllib
encoded_urls = ["Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG%2f120px-Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG"] * 10000

def decode_vector(urls):
  
  output = list()
  for url in urls:
    output.append(urllib.unquote(url).decode('utf8'))
"""
result = timeit.timeit("decode_vector(encoded_urls)", setup = s, number=100)/100
result = result*1000 #ms rather than s
