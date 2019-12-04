#include "Eigen/Core"
#include "Eigen/src/Core/Matrix.h"
#include <iostream>
#include <vector>

#define PREC 1e-5

class Symmetry_Operation
{

public:
    Symmetry_Operation(Eigen::Matrix3d cart_matrix,  Eigen::Vector3d translation);
    Eigen::Matrix3d cart_matrix;
    Eigen::Vector3d translation;

};


bool compare_diff_to_prec(Eigen::Vector3d difference) {
    int sig_diff = 0;
    if (difference.row(1) > PREC) {
       sig_diff++;
    }
    if (difference.row(2) > PREC) {
       sig_diff++;
    }
    if (difference.row(3) > PREC) {
       sig_diff++;
    }
    if (sig_diff != 0) {
       return true
    }
    else {
       return false
    }
} 

bool has_translation(Eigen::Vector3d translation, Eigen::Matrix3d lattice)
{// check if translation is 0 or integer multiple of lattice vectors-> false,
 // else true
    Eigen::Vector3d lattice_displacement = lattice.inverse() * translation;
    Eigen::Vector3d modification << 0.5, 0.5, 0.5;
    Eigen::Vector3d modif_displ = lattice_displacement + modification;
    Eigen::Vector3d trunc_displ = (int) modif_displ;
    Eigen::Vector3d difference = abs(lattice_displacement - trunc_displ);
    bool check_trans = compare_diff_to_prec(difference);
    if (check_trans == true) {
        return true;
    }
    else {
        return false;
    } 
}

int num_eigenvals_equal_one(Eigen::Matrix3d cart_matrix){
    // some function that evaluate eigne vlaue and counts how many ones
    double det;
    
    /// det (matrix -lambda*identity)==0, lambda is the eigenvalues
}

std::string check_op_type(Symmetry_Operation sym_op, Eigen::Matrix3d lattice)
{ // take in sym_op returns string of op type
    int trace = sym_op.cart_matrix.trace();
    std::string type;
    double det = sym_op.cart_matrix.determinant();

    if (trace == 3)
    {
        type = "Identity";
        return type;
    }
    if (trace == -3)
    {
        type = "Inversion";
        return type;
    }
    if (has_translation(sym_op.translation, lattice))
    {
        if (det == 1)
        {
            type = "Screw";
            return type;
        }
        else
        {
            type = "Glide";
            return type;
        }
    }
    if (det == 1)
    {
        type = "rotation";
        return type;
    }
    int neigen = num_eigenvals_equal_one(sym_op.cart_matrix);
    if (neigen == 2)
    {
        type = "Mirror";
        return type;
    }
    else if (neigen == 1)
    {
        type = "Improper rotation";
        return type;
    }
    else
    {
        type = "Error Type not idenitified!!!";
        return type;
    }
}

int main(int argc, char* argv[])
{ // WHAT is the actual input????
    // for now, hard coding example sym_ops

    Symmetry_Operation sym_op;
    std::string op_type = check_op_type(sym_op);
    std::cout << "This is a " << op_type << std::endl;
}
