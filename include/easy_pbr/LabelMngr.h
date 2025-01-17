#pragma once
//C++
#include <iosfwd>
#include <vector>
#include <unordered_map>
#include <memory>

#include <configuru.hpp>

//OpenCV
#include <opencv2/core/mat.hpp>

//Eigen
#include <Eigen/Core>

namespace easy_pbr{

class LabelMngr : public std::enable_shared_from_this<LabelMngr>{ //the enable shared from this is needed so that we can safely create shared ptr to a single object managed by both dataloaders and MeshCores
public:
    LabelMngr(const configuru::Config& config);
    LabelMngr(const std::string labels_file, const std::string colors_cheme_file, const std::string frequency_file, const int unlabeled_idx );
    LabelMngr(const int nr_classes, const int unlabeled_idx ); //creates some random colors for X nr of classes and set unlabeled idx to color black

    int nr_classes();
    // int get_idx_background(); //I know what it is but I don't care (in image segmentation it may be just the clutter behing an object that we don't care about labeling)
    int get_idx_unlabeled();  //idx of the class that is assigned to unabeled (corresponds to "I don't know"
    // int get_idx_invalid();    //idx of a class that will for sure never appear in the dataset, like -1 or nr_classes +1 (we use the latter for the reason that store then in unsigned data storage)
    std::string idx2label(int idx);
    int label2idx(std::string label);
    Eigen::Vector3d color_for_label(const int idx);
    void set_color_for_label_with_idx(const int idx, Eigen::Vector3d color); //modifies the color for one specific label at a certain index
    void set_color_scheme(Eigen::MatrixXd& color_per_class); //set a new colormap as a matrix of Nx3 where N is the number of classes we are interested in
    Eigen::MatrixXd color_scheme();
    Eigen::MatrixXf color_scheme_for_gl(); //the data in eigen is stord columnmajor and we want to put the rows first into gl
    Eigen::VectorXf class_frequencies();
    void compact(const std::string label_to_remove); //sometimes the labels come in a weird format in which the indices are not consecutive. For this we remove the labels that are marked as UNUSED
    void reindex_into_compacted_labels(Eigen::MatrixXi& labels_indices); //after making the labels indices consecutive using compact, the L_gt will have to be reindexed for each pointcloud
    void reindex_into_uncompacted_labels(Eigen::MatrixXi& labels_indices); //sometimes we want the labels to be uncompacted back
    cv::Mat apply_color_map(cv::Mat classes);

private:
    void init_params(const configuru::Config& config_file);
    void clear();
    void read_data(const std::string labels_file, const std::string color_scheme_file, const std::string frequency_file);
    int m_nr_classes;
    int m_unlabeled_idx;

    // std::string m_background_label;  //correspond to I know what this is but it's nothing from the training objects so it's set to objects
    // int m_unlabeled_idx; //corresponds to "I don't know" (usually used when clearing the pseudo_gt)
    std::vector<std::string> m_idx2label;
    std::unordered_map<std::string,int> m_label2idx;
    Eigen::MatrixXd m_C_per_class;
    Eigen::VectorXf m_frequency_per_class;
    std::vector<int> m_idx_uncompacted2idx_compacted;
    std::vector<int> m_idx_compacted2idx_uncompacted;

    //params
    // std::string m_labels_file;
    // std::string m_color_scheme_file;

}
;
} //namespace easy_pbr
