
#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;


// Checks the validity of a symmetry operation matrix.
bool is_sym_op_valid(Eigen::Matrix2d S) {
	Eigen::Matrix2d S_check = S.transpose()*S;
	if (S_check.isIdentity(1e-5) == false) {
		return false;
	}
	return true; 
}

//Creates grid points from lattice vectors a, b and a radius.
std::vector<Eigen::Vector2d> create_grid_points(Eigen::Matrix2d L, int radius) {
	std::vector<Eigen::Vector2d> list_gp;
	for (int n = -radius; n < (radius+1); n++) {
		for (int m = -radius; m < (radius+1); m++) {
			Eigen::Vector2d p = n * L.col(0) + m * L.col(1);
			list_gp.push_back(p);
		}
	}
	return list_gp;
}

//Calculates a list of L primes based on a lattice grid. 
std::vector<Eigen::Matrix2d> calc_L_primes(Eigen::Matrix2d L, int radius) {
	std::vector<Eigen::Matrix2d> list_Lps;
	std::vector<Eigen::Vector2d> ps = create_grid_points(L, radius);
	for (int p1 = 0; p1 < ps.size(); p1++) {
		for (int p2 = 0; p2 < ps.size(); p2++) {
			Eigen::Matrix2d Lp;
			Lp.col(0) << ps[p1];
			Lp.col(1) << ps[p2];
			list_Lps.push_back(Lp);
		}
	}
	return list_Lps;
}

//Calculates a point group (a list of symmetry operations) from L primes.
std::vector<Eigen::Matrix2d> calc_point_group(Eigen::Matrix2d L, int radius) {
	std::vector<Eigen::Matrix2d> list_sym_ops;
	std::vector<Eigen::Matrix2d> Lps = calc_L_primes(L, radius);
	for (int i = 0; i < Lps.size(); i++) {
		Eigen::Matrix2d Lp = Lps[i];
		Eigen::Matrix2d S = Lp * Lp.inverse();
		if (is_sym_op_valid(S) == true) {
			list_sym_ops.push_back(S);
		}
	}
	return list_sym_ops;
}

//Functor written to compare two matrices.
struct compare_sym_ops {
				compare_sym_ops(Eigen::Matrix2d Sp) : Sp(Sp) {}
				bool operator()(Eigen::Matrix2d sm) const {return Sp.isApprox(sm,1e-5);}

			private:
				Eigen::Matrix2d Sp;
			};

//Checks the validity of the list of symmetry operations as a group by checking for closure. 
bool group_is_closed(std::vector<Eigen::Matrix2d> list_sym_ops) {
	for (int s1 = 0; s1 < list_sym_ops.size(); s1++) {
		for (int s2 = 0; s2 < list_sym_ops.size(); s2++) {
			Eigen::Matrix2d Sp = list_sym_ops[s1] *list_sym_ops[s2];
			compare_sym_ops compare(Sp);
			if (std::find_if (list_sym_ops.begin(), list_sym_ops.end(), compare)==list_sym_ops.end()) {
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char *argv[]) {
       	
	Eigen::Matrix2d Lsquare;
	Lsquare.row(1) << 2.0, 0.0;
	Lsquare.row(2) << 0.0, 2.0;

	int radius = 5;

	std::vector<Eigen::Matrix2d> square_pt_grp = calc_point_group(Lsquare, radius);

	bool sqr_pt_grp_closed = group_is_closed(square_pt_grp);

	std::cout << "The symmetry operations are: " << square_pt_grp << "\n";
	
	if (sqr_pt_grp_closed != false) {

		std::cout << "The group is closed.\n";

	}


	
    return 0;
}
