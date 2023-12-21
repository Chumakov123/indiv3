#include "mesh.h"

std::vector<std::string> split(const std::string& s, const char delimiter) {
    size_t pos_start = 0, pos_end;
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        if (!token.empty()) {
            res.push_back(std::move(token));
        }
        pos_start = pos_end + 1;
    }

    token = s.substr(pos_start);
    if (!token.empty()) {
        res.push_back(std::move(token));
    }

    return res;
}

Mesh::Mesh(const char* meshPath)
{
    parseFile(meshPath);
    InitPositionBuffers();
}
Mesh::~Mesh() {
    if (glIsBuffer(VBO)) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBO);
    }
    if (glIsVertexArray(VAO)) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
    }
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept {
    if (this != &mesh) {
        vertices = std::move(mesh.vertices);
        VBO = mesh.VBO;
        VAO = mesh.VAO;
        mesh.VBO = 0;
        mesh.VAO = 0;
    }
    return *this;
}

Mesh::Mesh(Mesh&& mesh) noexcept {
    vertices = std::move(mesh.vertices);
    VBO = mesh.VBO;
    VAO = mesh.VAO;
    mesh.VBO = 0;
    mesh.VAO = 0;
}

void Mesh::parseFile(const std::string& filePath)
{
    try {
        std::ifstream obj(filePath);

        if (!obj.is_open()) {
            throw std::exception("File cannot be opened");
        }

        std::vector<std::vector<float>> v, vt, vn;
        std::string line;

        std::istringstream iss;

        auto parseValues = [&iss](const std::string& data) {
            iss.str(data);
            std::vector<float> values;
            std::string value;
            while (iss >> value) {
                values.emplace_back(std::stof(value));
            }
            iss.clear();
            return values;
        };

        while (std::getline(obj, line)) {
            iss.str(line);
            std::string type;
            iss >> type;

            if (type == "v")        v.emplace_back(parseValues(line.substr(2)));
            else if (type == "vn")  vn.emplace_back(parseValues(line.substr(3)));
            else if (type == "vt")  vt.emplace_back(parseValues(line.substr(3)));
            else if (type == "f") {
                auto splitted = split(line, ' ');

                auto processFaceVertex = [&](const std::string& vertex) {
                    auto verts = split(vertex, '/');
                    int positionIndex = std::stoi(verts[0]) - 1;
                    int textureIndex = (verts.size() > 1 && !verts[1].empty()) ? std::stoi(verts[1]) - 1 : -1;
                    int normaleIndex = (verts.size() > 2) ? std::stoi(verts[2]) - 1 : -1;

                    MeshVertex vertexData;
                    for (int j = 0; j < 3; ++j)
                        vertexData.position[j] = v[positionIndex][j];
                    for (int j = 0; j < 3; ++j)
                        vertexData.normal[j] = (normaleIndex != -1) ? vn[normaleIndex][j] : 0.0f;
                    for (int j = 0; j < 2; ++j)
                        vertexData.texture[j] = (textureIndex != -1) ? vt[textureIndex][j] : 0.0f;

                    vertices.emplace_back(vertexData);
                };

                // Обработка треугольников и прямоугольников
                if (splitted.size() >= 4) {
                    for (size_t i = 1; i <= 3; ++i) {
                        processFaceVertex(splitted[i]);
                    }

                    if (splitted.size() == 5) {
                        processFaceVertex(splitted[1]);
                        processFaceVertex(splitted[3]);
                        processFaceVertex(splitted[4]);
                    }
                }
            }
            iss.clear();
        }
        std::cout << filePath << " has been loaded. Total vertices: " << vertices.size() << std::endl;
        return;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void Mesh::InitPositionBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, texture));

    glBindVertexArray(0);
}