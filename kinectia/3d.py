import struct

def write_string(file, string):
    string_bytes = string.encode('utf-8')  # Convertir la cadena a bytes
    file.write(len(string_bytes).to_bytes(4, byteorder='little'))
    file.write(string_bytes)

def write_vertices(file, vertices):
    file.write(b'Vert')
    file.write(len(vertices).to_bytes(4, byteorder='little'))
    for vertex in vertices:
        file.write(struct.pack('fff', *vertex))

def write_faces(file, faces):
    file.write(b'Face')
    file.write(len(faces).to_bytes(4, byteorder='little'))
    for face in faces:
        file.write(len(face).to_bytes(4, byteorder='little'))
        file.write(struct.pack('I' * len(face), *face))

blend_file_path = 'mi_escena.blend'
blend_header = b'BLENDER-v279'

cube_data = {
    'name': 'Cube',
    'type': 'MESH',
    'vertices': [(0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)],
    'faces': [(0, 1, 2, 3)],
}

with open(blend_file_path, 'wb') as blend_file:
    blend_file.write(blend_header)
    write_string(blend_file, 'MESH' + '\x00' + cube_data['name'])
    write_vertices(blend_file, cube_data['vertices'])
    write_faces(blend_file, cube_data['faces'])
