#pragma once

#include "Constants.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"
#include "json.hpp"

namespace Karbon
{
    struct Matrix4
    {

        [[nodiscard]] constexpr Matrix4(){};

        [[nodiscard]] constexpr Matrix4(const float *array, [[maybe_unused]] int array_size)
        {
            assert(16 == array_size);

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    this->_matrix[i][j] = array[i * 4 + j];
        }

        [[nodiscard]] constexpr Matrix4(const std::array<std::array<float, 4>, 4> &array)
        {
            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    this->_matrix[i][j] = array[i][j];
        }

        [[nodiscard]] constexpr Matrix4(const std::vector<std::vector<float>> &values)
        {
            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    this->_matrix[i][j] = values[i][j];
        }

        // constructor for (float, float, float, int, float, float, float, int, float, float, float, float, float, float, float, float)
        [[nodiscard]] constexpr Matrix4(const float a00, const float a01, const float a02, const float a03, const float a10, const float a11, const float a12, const float a13, const float a20, const float a21, const float a22, const float a23, const float a30, const float a31, const float a32, const float a33)
        {
            this->_matrix[0][0] = a00;
            this->_matrix[0][1] = a01;
            this->_matrix[0][2] = a02;
            this->_matrix[0][3] = a03;
            this->_matrix[1][0] = a10;
            this->_matrix[1][1] = a11;
            this->_matrix[1][2] = a12;
            this->_matrix[1][3] = a13;
            this->_matrix[2][0] = a20;
            this->_matrix[2][1] = a21;
            this->_matrix[2][2] = a22;
            this->_matrix[2][3] = a23;
            this->_matrix[3][0] = a30;
            this->_matrix[3][1] = a31;
            this->_matrix[3][2] = a32;
            this->_matrix[3][3] = a33;
        }

        // [] operator
        [[nodiscard]] constexpr float *operator[](const int index) noexcept
        {
            return this->_matrix[index];
        }

        // / operator
        [[nodiscard]] constexpr Matrix4 operator/(const float rhs) const noexcept
        {
            return Matrix4(
                this->_matrix[0][0] / rhs, this->_matrix[0][1] / rhs, this->_matrix[0][2] / rhs, this->_matrix[0][3] / rhs,
                this->_matrix[1][0] / rhs, this->_matrix[1][1] / rhs, this->_matrix[1][2] / rhs, this->_matrix[1][3] / rhs,
                this->_matrix[2][0] / rhs, this->_matrix[2][1] / rhs, this->_matrix[2][2] / rhs, this->_matrix[2][3] / rhs,
                this->_matrix[3][0] / rhs, this->_matrix[3][1] / rhs, this->_matrix[3][2] / rhs, this->_matrix[3][3] / rhs);
        }

        // == operator
        [[nodiscard]] constexpr bool operator==(const Matrix4 &other) const noexcept
        {
            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    if ((std::abs(this->_matrix[i][j] - other._matrix[i][j]) <= 0.00001))
                        return false;

            return true;
        }

        // != operator
        [[nodiscard]] constexpr bool operator!=(const Matrix4 &other) const noexcept
        {
            return !(*this == other);
        }

        // + operator
        [[nodiscard]] constexpr Matrix4 operator+(const Matrix4 &other) const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[i][j] + other._matrix[i][j];

            return result;
        }

        // - operator
        [[nodiscard]] constexpr Matrix4 operator-(const Matrix4 &other) const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[i][j] - other._matrix[i][j];

            return result;
        }

        // * by std::vector<std::vector<float>>
        [[nodiscard]] constexpr Matrix4 operator*(const std::vector<std::vector<float>> &other) const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[i][0] * other[0][j] + this->_matrix[i][1] * other[1][j] + this->_matrix[i][2] * other[2][j] + this->_matrix[i][3] * other[3][j];

            return result;
        }
        // * operator
        [[nodiscard]] constexpr Matrix4 operator*(const Matrix4 &other) const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[i][0] * other._matrix[0][j] +
                                           this->_matrix[i][1] * other._matrix[1][j] +
                                           this->_matrix[i][2] * other._matrix[2][j] +
                                           this->_matrix[i][3] * other._matrix[3][j];

            return result;
        }

        // * operator
        [[nodiscard]] constexpr Matrix4 operator*(const float &other) const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[i][j] * other;

            return result;
        }

        // multiply vector3 by matrix and return a vector3
        [[nodiscard]] constexpr Karbon::Vector operator*(const Karbon::Vector &other) const noexcept
        {
            Karbon::Vector result;

            result.x = this->_matrix[0][0] * other.x + this->_matrix[0][1] * other.y +
                       this->_matrix[0][2] * other.z;
            result.y = this->_matrix[1][0] * other.x + this->_matrix[1][1] * other.y +
                       this->_matrix[1][2] * other.z;
            result.z = this->_matrix[2][0] * other.x + this->_matrix[2][1] * other.y +
                       this->_matrix[2][2] * other.z;

            return result;
        }

        [[nodiscard]] constexpr Karbon::Point operator*(const Karbon::Point &other) const noexcept
        {
            Karbon::Point result;

            result.x = this->_matrix[0][0] * other.x + this->_matrix[0][1] * other.y +
                       this->_matrix[0][2] * other.z + this->_matrix[0][3];
            result.y = this->_matrix[1][0] * other.x + this->_matrix[1][1] * other.y +
                       this->_matrix[1][2] * other.z + this->_matrix[1][3];
            result.z = this->_matrix[2][0] * other.x + this->_matrix[2][1] * other.y +
                       this->_matrix[2][2] * other.z + this->_matrix[2][3];

            return result;
        }

        // *= operator
        [[nodiscard]] constexpr Matrix4 &operator*=(const Matrix4 &other) noexcept
        {
            *this = *this * other;

            return *this;
        }

        // *= operator
        [[nodiscard]] constexpr Matrix4 &operator*=(const float &other) noexcept
        {
            *this = *this * other;

            return *this;
        }

        // *= operator
        [[nodiscard]] constexpr Matrix4 &operator+=(const Matrix4 &other) noexcept
        {
            *this = *this + other;

            return *this;
        }

        // *= operator
        [[nodiscard]] constexpr Matrix4 &operator-=(const Matrix4 &other) noexcept
        {
            *this = *this - other;

            return *this;
        }

        // [] operator
        [[nodiscard]] constexpr const float &operator[](const int index) const noexcept
        {
            return this->_matrix[index / 4][index % 4];
        }

        // [] operator
        [[nodiscard]] constexpr const float &operator()(const int row, const int column) const noexcept
        {
            return this->_matrix[row][column];
        }

        // transpose
        [[nodiscard]] constexpr Matrix4 transpose() const noexcept
        {
            Matrix4 result;

            for (char i = 0; i < 4; i++)
                for (char j = 0; j < 4; j++)
                    result._matrix[i][j] = this->_matrix[j][i];

            return result;
        }

        // determinant
        [[nodiscard]] constexpr float determinant4() const noexcept
        {
            return this->_matrix[0][0] * this->_matrix[1][1] * this->_matrix[2][2] * this->_matrix[3][3] +
                   this->_matrix[0][0] * this->_matrix[1][2] * this->_matrix[2][3] * this->_matrix[3][1] +
                   this->_matrix[0][0] * this->_matrix[1][3] * this->_matrix[2][1] * this->_matrix[3][2] +
                   this->_matrix[0][1] * this->_matrix[1][0] * this->_matrix[2][3] * this->_matrix[3][2] +
                   this->_matrix[0][1] * this->_matrix[1][2] * this->_matrix[2][0] * this->_matrix[3][3] +
                   this->_matrix[0][1] * this->_matrix[1][3] * this->_matrix[2][2] * this->_matrix[3][0] +
                   this->_matrix[0][2] * this->_matrix[1][0] * this->_matrix[2][1] * this->_matrix[3][3] +
                   this->_matrix[0][2] * this->_matrix[1][1] * this->_matrix[2][3] * this->_matrix[3][0] +
                   this->_matrix[0][2] * this->_matrix[1][3] * this->_matrix[2][0] * this->_matrix[3][1] +
                   this->_matrix[0][3] * this->_matrix[1][0] * this->_matrix[2][2] * this->_matrix[3][1] +
                   this->_matrix[0][3] * this->_matrix[1][1] * this->_matrix[2][0] * this->_matrix[3][2] +
                   this->_matrix[0][3] * this->_matrix[1][2] * this->_matrix[2][1] * this->_matrix[3][0] -
                   this->_matrix[0][0] * this->_matrix[1][1] * this->_matrix[2][3] * this->_matrix[3][2] -
                   this->_matrix[0][0] * this->_matrix[1][2] * this->_matrix[2][1] * this->_matrix[3][3] -
                   this->_matrix[0][0] * this->_matrix[1][3] * this->_matrix[2][2] * this->_matrix[3][1] -
                   this->_matrix[0][1] * this->_matrix[1][0] * this->_matrix[2][2] * this->_matrix[3][3] -
                   this->_matrix[0][1] * this->_matrix[1][2] * this->_matrix[2][3] * this->_matrix[3][0] -
                   this->_matrix[0][1] * this->_matrix[1][3] * this->_matrix[2][0] * this->_matrix[3][2] -
                   this->_matrix[0][2] * this->_matrix[1][0] * this->_matrix[2][3] * this->_matrix[3][1] -
                   this->_matrix[0][2] * this->_matrix[1][1] * this->_matrix[2][0] * this->_matrix[3][3] -
                   this->_matrix[0][2] * this->_matrix[1][3] * this->_matrix[2][1] * this->_matrix[3][0] -
                   this->_matrix[0][3] * this->_matrix[1][0] * this->_matrix[2][1] * this->_matrix[3][2] -
                   this->_matrix[0][3] * this->_matrix[1][1] * this->_matrix[2][2] * this->_matrix[3][0] -
                   this->_matrix[0][3] * this->_matrix[1][2] * this->_matrix[2][0] * this->_matrix[3][1];
        }

        template <size_t size>
        [[nodiscard]] constexpr void sub_matrix(const float (&matrix)[size + 1][size + 1], const int row, const int column, float (&sub_matrix)[size][size]) const
        {
            bool a_replaced = false;
            bool b_replaced = false;

            for (char i = 0; i < size + 1; i++)
            {
                b_replaced = false;
                if (i != row)
                    for (char j = 0; j < size + 1; j++)
                    {
                        if (j != column)
                            sub_matrix[a_replaced ? i - 1 : i][b_replaced ? j - 1 : j] = matrix[i][j];
                        else
                            b_replaced = true;
                    }
                else
                    a_replaced = true;
            }
        }

        [[nodiscard]] float constexpr determinant(const float (&matrix)[2][2]) const
        {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }

        [[nodiscard]] float constexpr determinant(const float (&matrix)[3][3]) const
        {
            float det = 0;

            for (char i = 0; i < 3; i++)
            {
                if (i % 2 == 0)
                {
                    float temp_matrix[2][2];
                    sub_matrix<2>(matrix, i, 0, temp_matrix);
                    det += matrix[i][0] * determinant(temp_matrix);
                }
                else
                {
                    float temp_matrix[2][2];
                    sub_matrix<2>(matrix, i, 0, temp_matrix);
                    det -= matrix[i][0] * determinant(temp_matrix);
                }
            }

            return det;
        }

        // cofactor
        [[nodiscard]] constexpr Matrix4 cofactor() const noexcept
        {
            Matrix4 cofactor;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    float temp_matrix[3][3];
                    sub_matrix<3>(_matrix, i, j, temp_matrix);

                    cofactor[i][j] = (float)pow(-1, i + j) * determinant(temp_matrix);
                }
            }

            return cofactor;
        }

        // matrix adjugate
        [[nodiscard]] constexpr Matrix4 adjugate() const noexcept
        {
            return this->cofactor().transpose();
        }

        // matrix inverse
        [[nodiscard]] constexpr Matrix4 inverse() const noexcept
        {
            return this->adjugate() / this->determinant4();
        }

        [[nodiscard]] Matrix4 translate(std::vector<float> const &values) const
        {
            std::vector<std::vector<float>> translation_values = {
                {1, 0, 0, values[0]},
                {0, 1, 0, values[1]},
                {0, 0, 1, values[2]},
                {0, 0, 0, 1},
            };

            return *this * translation_values;
        }

        [[nodiscard]] Matrix4 translate(const float x, const float y, const float z) const
        {
            std::vector<std::vector<float>> translation_values = {
                {1, 0, 0, x},
                {0, 1, 0, y},
                {0, 0, 1, z},
                {0, 0, 0, 1},
            };

            return *this * translation_values;
        }

        [[nodiscard]] Matrix4 scale(std::vector<float> const &values) const
        {
            std::vector<std::vector<float>> translation_values = {
                {values[0], 0, 0, 0},
                {0, values[1], 0, 0},
                {0, 0, values[2], 0},
                {0, 0, 0, 1},
            };

            return *this * translation_values;
        }

        [[nodiscard]] Matrix4 scale(const float x, const float y, const float z) const
        {
            std::vector<std::vector<float>> translation_values = {
                {x, 0, 0, 0},
                {0, y, 0, 0},
                {0, 0, z, 0},
                {0, 0, 0, 1},
            };

            return *this * translation_values;
        }

        [[nodiscard]] Matrix4 rotate_x(const float radians) const
        {
            std::vector<std::vector<float>> rotate_x_values = {
                {1, 0, 0, 0},
                {0, std::cos(radians), -std::sin(radians), 0},
                {0, std::sin(radians), std::cos(radians), 0},
                {0, 0, 0, 1},
            };

            return *this * rotate_x_values;
        }

        [[nodiscard]] Matrix4 rotate_y(const float radians) const
        {
            std::vector<std::vector<float>> rotate_y_values = {
                {std::cos(radians), 0, std::sin(radians), 0},
                {0, 1, 0, 0},
                {-std::sin(radians), 0, std::cos(radians), 0},
                {0, 0, 0, 1},
            };

            return *this * rotate_y_values;
        }

        [[nodiscard]] Matrix4 rotate_z(const float radians) const
        {
            std::vector<std::vector<float>> rotate_z_values = {
                {std::cos(radians), -std::sin(radians), 0, 0},
                {std::sin(radians), std::cos(radians), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1},
            };

            return *this * rotate_z_values;
        }

        [[nodiscard]] Matrix4 rotate(const float radians_x, const float radians_y, const float radians_z) const
        {
            return this->rotate_x(radians_x).rotate_y(radians_y).rotate_z(radians_z);
        }

        [[nodiscard]] Matrix4 shear(float Xy, float Xz, float Yx, float Yz, float Zx, float Zy) const
        {
            std::vector<std::vector<float>> shear_values = {
                {1, Xy, Xz, 0},
                {Yx, 1, Yz, 0},
                {Zx, Zy, 1, 0},
                {0, 0, 0, 1},
            };

            return *this * shear_values;
        }

        // << operator
        friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m)
        {
            os << "[" << m._matrix[0][0] << " " << m._matrix[0][1] << " " << m._matrix[0][2] << " " << m._matrix[0][3] << "]" << std::endl;
            os << "[" << m._matrix[1][0] << " " << m._matrix[1][1] << " " << m._matrix[1][2] << " " << m._matrix[1][3] << "]" << std::endl;
            os << "[" << m._matrix[2][0] << " " << m._matrix[2][1] << " " << m._matrix[2][2] << " " << m._matrix[2][3] << "]" << std::endl;
            os << "[" << m._matrix[3][0] << " " << m._matrix[3][1] << " " << m._matrix[3][2] << " " << m._matrix[3][3] << "]" << std::endl;

            return os;
        }

        // >> operator
        friend std::istream &operator>>(std::istream &is, Matrix4 &m)
        {
            for (char i = 0; i < 4; i++)
            {
                for (char j = 0; j < 4; j++)
                {
                    is >> m._matrix[i][j];
                }
            }

            return is;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json_matrix;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    json_matrix[i][j] = _matrix[i][j];
                }
            }

            return json_matrix.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static Matrix4 from_json(const std::string &json_string)
        {
            nlohmann::json json_matrix = nlohmann::json::parse(json_string);

            Matrix4 matrix;

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    matrix._matrix[i][j] = json_matrix[i][j];
                }
            }

            return matrix;
        }

    private:
        float _matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    };

    constexpr const Matrix4 IDENTITY = Matrix4();
}; // namespace CLOAL