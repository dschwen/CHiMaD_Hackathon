#!/usr/bin/env python
import chigger
import sys
import vtk

base = sys.argv[1]
var = sys.argv[2]

reader = chigger.exodus.ExodusReader(base + '.e')
reader.update()

camera = vtk.vtkCamera()
camera.SetViewUp(0.0000, 1.0000, 0.0000)
camera.SetPosition(15.0000, 3.0000, 11.5)
camera.SetFocalPoint(15.0000, 3.0000, 0.0000)

# data = chigger.exodus.ExodusResult(reader, variable='c', cmap='viridis')
data = chigger.exodus.ExodusResult(reader, variable=var, cmap='viridis', camera=camera)
cbar = chigger.exodus.ExodusColorBar(data)
# window = chigger.RenderWindow(data, cbar, size=[600,600], background=[1, 1, 1], test=True)
window = chigger.RenderWindow(data, cbar, size=[1200,240], background=[1, 1, 1], test=True)

t = reader.getTimes()
ii = 0
for i in range(0, len(t)):
    reader.setOptions(timestep = i)
    window.write('%s.%s.png' % (base, var))
    ii += 1

window.start()

