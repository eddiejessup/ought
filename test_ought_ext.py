import ought_ext
net = ought_ext.Network(10, 10)
net.iterate()
print(net.to_json())
