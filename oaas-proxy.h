#pragma once

#include <optional>
#include <variant>

#include <optional>
#include <stdexcept>
#include <regex>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <map>

/**
 * An object that represents a reference to a resource by its name and its URI allowing
 * references to resources in the semantic web.
 */
class Resource {
public:
    Resource() = default;
    virtual ~Resource() = default;

private:
    std::optional<std::map<std::wstring, std::wstring>> name;
    std::optional<std::wstring> uri;

public:
    /**
     * Determines the name of the resource using a language string.
     */
    std::optional<std::map<std::wstring, std::wstring>> get_name() const { return name; }
    void set_name(std::optional<std::map<std::wstring, std::wstring>> value) { this->name = value; }

    /**
     * Determines the Uniform Resource Identifier (URI) of the resource using a string.
     */
    std::optional<std::wstring> get_uri() const { return uri; }
    void set_uri(std::optional<std::wstring> value) { this->uri = value; }
};

enum class AggregationKind : int { COMPOSITE, NONE, SHARED };

enum class RestrictedTo : int { ABSTRACT, COLLECTIVE, EVENT, EXTRINSIC_MODE, FUNCTIONAL_COMPLEX, INTRINSIC_MODE, QUALITY, QUANTITY, RELATOR, SITUATION, TYPE };

using Text = std::variant<std::map<std::wstring, std::wstring>, std::wstring>;

enum class ElementType : int { BINARY_RELATION, BINARY_RELATION_VIEW, CLASS, CLASS_VIEW, DIAGRAM, GENERALIZATION, GENERALIZATION_SET, GENERALIZATION_SET_VIEW, GENERALIZATION_VIEW, LINK, LINK_VIEW, NARY_RELATION, NARY_RELATION_VIEW, NOTE, NOTE_VIEW, PACKAGE_VIEW };

/**
 * A named element that contains the visual representation (i.e., the concrete syntax) of an
 * OntoUML model or of a portion of it.
 *
 * An OntoUML element that represents a single model element in a diagram.
 *
 * A view element connects a model element to the shapes in a diagram necessary to represent
 * a single occurrence of it. For example, an n-ary relation view connects a single relation
 * element to one diamond and a set of paths that represent a single occurrence of it in a
 * diagram. Multiple views can represent multiple occurrences of an element in the same
 * diagram.
 *
 * A view element is responsible for what portions of a model element are present in a
 * single diagram representation (e.g., whether the cardinality of a property is shown),
 * unlike a shape, which is responsible for aspects of the actual drawing (e.g., how to
 * render a portion of a view, in which position, and with which dimensions).
 *
 * A view element that represents the single occurrence of a class in a diagram.
 *
 * A view element that represents the single occurrence of a generalization set in a
 * diagram.
 *
 * A view element that represents the single occurrence of a n-ary relation in a diagram.
 *
 * A view element that represents the single occurrence of a note in a diagram.
 *
 * A view element that represents the single occurrence of a package in a diagram.
 *
 * A view element that represents the single occurrence of a binary connector (e.g., a
 * binary relation, or a generalization) in a diagram.
 *
 * A view element that represents the single occurrence of a binary relation in a diagram.
 *
 * A view element that represents the single occurrence of a link in a diagram.
 *
 * A model element that can be grouped into a package.
 *
 * A decoratable element (either a class or a relation) that defines properties exhibited by
 * its instances.
 *
 * A classifier that defines the properties of a set of "individualized" entities (i.e.,
 * non-relational) of the subject domain.
 *
 * Examples include "Person", "Enrollment", and "Grade".
 *
 * The instances of a class may include entities such as objects (e.g., people,
 * organizations, vehicles), reified properties (e.g., leafs' colors, agents' intentions,
 * enrollments), and bare values (e.g., a number or a literal).
 *
 * A relation that defines the properties of a set of binary relations of the subject
 * domain.
 *
 * Examples include "studies in", and derivation relations (e.g., between material relations
 * and relators).
 *
 * A binary relation may either connect two classes, or a relation (as source) and a class
 * (as target) in the case of derivation relations connecting descriptive relations to the
 * classes that serve as their truthmakers (as in the relation between the material relation
 * "studies in" and the "Enrollment" relator).
 *
 * A relation that defines the properties of a set of relations of the subject domain that
 * connect more than two members.
 *
 * Examples include "studies in", "buys product from" (ternary relation), and derivation
 * relations (e.g., between material relations and relators).
 *
 * A model element that represents the generalization of a specific classifier into a
 * general classifier. When read in the inverse direction, a generalization is referred to
 * as a specialization.
 *
 * Examples include the generalization of a specific class "Student" into a general class
 * "Person," and the generalization of a specific relation "close friends with" into a
 * general relation "friends with".
 *
 * A generalization can only connect two classifiers of the same type, i.e., it can either
 * connect two class elements or two relation elements.
 *
 * A model element that represents a group of connected generalization elements. A
 * generalization set can define disjoint and/or complete constraints over the
 * generalizations it groups.
 *
 * Examples include the incomplete (i.e., non-complete) and overlapping (i.e., non-disjoint)
 * generalization set of "Person" into "Student" and "Teacher", and the disjoint and
 * complete generalization set of "Person" into "Child" and "Adult".
 *
 * All generalizations in the generalization set must share a common general classifier.
 *
 * A model element that connects a note to a model element it concerns.
 *
 * A model element that contains an annotation about the ontology or some of its elements. A
 * note can also be used to represent a constraint in both natural or structured language
 * (i.e., first-order logic, or OCL).
 */
class Element {
public:
    Element() :
        is_view_of_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        rectangle_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        diamond_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        path_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        source_view_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        target_view_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        general_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        specific_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        element_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        note_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt)
    {}
    virtual ~Element() = default;

private:
    std::optional<std::wstring> owner;
    ElementType type;
    std::optional<std::vector<std::wstring>> views;
    std::optional<std::wstring> is_view_of;
    ClassMemberConstraints is_view_of_constraint;
    std::optional<std::wstring> rectangle;
    ClassMemberConstraints rectangle_constraint;
    std::optional<std::vector<std::wstring>> generalizations;
    std::optional<Text> text;
    std::optional<std::wstring> diamond;
    ClassMemberConstraints diamond_constraint;
    std::optional<std::vector<std::wstring>> members;
    std::optional<std::vector<std::wstring>> paths;
    std::optional<std::wstring> path;
    ClassMemberConstraints path_constraint;
    std::optional<std::wstring> source_view;
    ClassMemberConstraints source_view_constraint;
    std::optional<std::wstring> target_view;
    ClassMemberConstraints target_view_constraint;
    std::optional<bool> is_abstract;
    std::optional<std::vector<std::wstring>> properties;
    std::optional<bool> is_powertype;
    std::optional<std::vector<std::wstring>> literals;
    std::optional<std::wstring> order;
    std::optional<std::vector<RestrictedTo>> restricted_to;
    std::optional<std::wstring> general;
    ClassMemberConstraints general_constraint;
    std::optional<std::wstring> specific;
    ClassMemberConstraints specific_constraint;
    std::optional<std::wstring> categorizer;
    std::optional<bool> is_complete;
    std::optional<bool> is_disjoint;
    std::optional<std::wstring> element;
    ClassMemberConstraints element_constraint;
    std::optional<std::wstring> note;
    ClassMemberConstraints note_constraint;

public:
    /**
     * Identifies the model element that is the owner of a diagram.
     */
    std::optional<std::wstring> get_owner() const { return owner; }
    void set_owner(std::optional<std::wstring> value) { this->owner = value; }

    /**
     * Determines the type of a diagram object.
     *
     * Determines the type of a class view object.
     *
     * Determines the type of a generalization set view object.
     *
     * Determines the type of a n-ary relation view object.
     *
     * Determines the type of a note view object.
     *
     * Determines the type of a package view object.
     *
     * Determines the type of a generalization view object.
     *
     * Determines the type of a link view object.
     *
     * Determines the type of a binary relation view object.
     *
     * Determines the type of a class object.
     *
     * Determines the type of a binary relation object.
     *
     * Determines the type of a n-ary relation object.
     *
     * Determines the type of a generalization object.
     *
     * Determines the type of a generalization set object.
     *
     * Determines the type of a link object.
     *
     * Determines the type of a note object.
     */
    const ElementType& get_type() const { return type; }
    ElementType& get_mutable_type() { return type; }
    void set_type(const ElementType& value) { this->type = value; }

    /**
     * Identifies the views contained in the diagram.
     */
    std::optional<std::vector<std::wstring>> get_views() const { return views; }
    void set_views(std::optional<std::vector<std::wstring>> value) { this->views = value; }

    /**
     * Identifies the model element that the view represents in the diagram.
     */
    std::optional<std::wstring> get_is_view_of() const { return is_view_of; }
    void set_is_view_of(std::optional<std::wstring> value) { if (value) CheckConstraint(L"is_view_of", is_view_of_constraint, *value); this->is_view_of = value; }

    /**
     * Identifies the rectangle shape that renders the class view in the diagram.
     *
     * Identifies the rectangle shape that renders the package view in the diagram.
     */
    std::optional<std::wstring> get_rectangle() const { return rectangle; }
    void set_rectangle(std::optional<std::wstring> value) { if (value) CheckConstraint(L"rectangle", rectangle_constraint, *value); this->rectangle = value; }

    /**
     * Identifies the generalization views that are grouped by the generalization set view in
     * the diagram.
     *
     * Identifies all generalizations that are involved by the generalization set.
     */
    std::optional<std::vector<std::wstring>> get_generalizations() const { return generalizations; }
    void set_generalizations(std::optional<std::vector<std::wstring>> value) { this->generalizations = value; }

    /**
     * Identifies the text shape that renders the generalization set view in the diagram.
     *
     * Identifies the text shape that renders the note view in the diagram.
     *
     * Determines the contents of a note using a language string.
     */
    std::optional<Text> get_text() const { return text; }
    void set_text(std::optional<Text> value) { this->text = value; }

    /**
     * Identifies the diamond shape that renders the joining of all paths of the n-ary relation
     * in the diagram.
     */
    std::optional<std::wstring> get_diamond() const { return diamond; }
    void set_diamond(std::optional<std::wstring> value) { if (value) CheckConstraint(L"diamond", diamond_constraint, *value); this->diamond = value; }

    /**
     * Identifies the class views (i.e., the members) that are connected by the n-ary relation
     * view in the diagram. This array of member views must be ordered according to the
     * properties of the relation the view represents.
     */
    std::optional<std::vector<std::wstring>> get_members() const { return members; }
    void set_members(std::optional<std::vector<std::wstring>> value) { this->members = value; }

    /**
     * Identifies the path shapes that render each path of the n-ary relation view in the
     * diagram. This array of paths must be ordered according to the properties of the relation
     * the view represents.
     */
    std::optional<std::vector<std::wstring>> get_paths() const { return paths; }
    void set_paths(std::optional<std::vector<std::wstring>> value) { this->paths = value; }

    /**
     * Identifies the path shape that renders the binary connector in the diagram.
     */
    std::optional<std::wstring> get_path() const { return path; }
    void set_path(std::optional<std::wstring> value) { if (value) CheckConstraint(L"path", path_constraint, *value); this->path = value; }

    /**
     * Identifies the source view the binary connector view connects in the diagram.
     */
    std::optional<std::wstring> get_source_view() const { return source_view; }
    void set_source_view(std::optional<std::wstring> value) { if (value) CheckConstraint(L"source_view", source_view_constraint, *value); this->source_view = value; }

    /**
     * Identifies the target view the binary connector view connects in the diagram.
     */
    std::optional<std::wstring> get_target_view() const { return target_view; }
    void set_target_view(std::optional<std::wstring> value) { if (value) CheckConstraint(L"target_view", target_view_constraint, *value); this->target_view = value; }

    /**
     * Determines whether the classifier can have direct instances using a boolean. Abstract
     * classifiers can only have instances when these are instances of some other classifier
     * that is not abstract (i.e., concrete) and is a specialization of the abstract one.
     */
    std::optional<bool> get_is_abstract() const { return is_abstract; }
    void set_is_abstract(std::optional<bool> value) { this->is_abstract = value; }

    /**
     * Identifies the properties contained in a classifier. These properties are referred to as
     * attributes when contained by classes, and relation ends when contained by relations. In
     * the case of relations, the properties array must be ordered.
     */
    std::optional<std::vector<std::wstring>> get_properties() const { return properties; }
    void set_properties(std::optional<std::vector<std::wstring>> value) { this->properties = value; }

    /**
     * Determines whether the high-order class is a "Cardelli powertype" using a boolean. In
     * other words, determines whether the high-order class is defined as the one whose
     * instances are its base type plus all possible specializations of it.
     */
    std::optional<bool> get_is_powertype() const { return is_powertype; }
    void set_is_powertype(std::optional<bool> value) { this->is_powertype = value; }

    /**
     * Identifies the literals of an enumeration class.
     */
    std::optional<std::vector<std::wstring>> get_literals() const { return literals; }
    void set_literals(std::optional<std::vector<std::wstring>> value) { this->literals = value; }

    /**
     * Determines the instantiation order of a class using a string. Examples include ordered
     * classes such as first-order classes (order "1"), second-order classes (order "2"), and
     * third-order classes (order "3"), as well as orderless classes (order "*").
     */
    std::optional<std::wstring> get_order() const { return order; }
    void set_order(std::optional<std::wstring> value) { this->order = value; }

    /**
     * Determines the possible ontological natures of the instances of a class using an array of
     * enumerated strings.
     *
     * Examples include the class "Vehicle" restricted to having "functional-complex" instances
     * and the class "Insured Item" restricted to "functional-complex" and "relator" (e.g.,
     * employment insurance).
     */
    std::optional<std::vector<RestrictedTo>> get_restricted_to() const { return restricted_to; }
    void set_restricted_to(std::optional<std::vector<RestrictedTo>> value) { this->restricted_to = value; }

    /**
     * Identifies the general classifier in a generalization element. E.g., in the
     * generalization of "Student" into "Person", "Person" is the general classifier.
     */
    std::optional<std::wstring> get_general() const { return general; }
    void set_general(std::optional<std::wstring> value) { if (value) CheckConstraint(L"general", general_constraint, *value); this->general = value; }

    /**
     * Identifies the general classifier in a generalization element. E.g., in the
     * generalization of "Student" into "Person", "Student" is the specific classifier.
     */
    std::optional<std::wstring> get_specific() const { return specific; }
    void set_specific(std::optional<std::wstring> value) { if (value) CheckConstraint(L"specific", specific_constraint, *value); this->specific = value; }

    /**
     * Identifies the high-order class that classifies (i.e., is instantiated by) every specific
     * class in the generalization set.
     *
     * For example, "Academic Role" as the categorizer of the generalization set of "Person"
     * into "Student" and "Teacher" representing the specific classes as instances of the
     * categorizer.
     *
     * A categorizer can only be present in generalization sets involving exclusively classes.
     */
    std::optional<std::wstring> get_categorizer() const { return categorizer; }
    void set_categorizer(std::optional<std::wstring> value) { this->categorizer = value; }

    /**
     * Determines whether the specific classifiers in the generalization set completely cover
     * the extension of the general classifier.
     *
     * Examples include the generalization set involving "Child" and "Adult" generalized into
     * "Person", where the "is complete" as "true" indicates that every person is either an
     * instance of "Child" or "Adult."
     */
    std::optional<bool> get_is_complete() const { return is_complete; }
    void set_is_complete(std::optional<bool> value) { this->is_complete = value; }

    /**
     * Determines whether the specific classifiers in the generalization set have disjoint
     * extensions.
     *
     * Examples include the generalization set involving "Child" and "Adult" generalized into
     * "Person", where the "is disjoint" as "true" indicates that no person is simultaneously an
     * instance of "Child" and "Adult."
     */
    std::optional<bool> get_is_disjoint() const { return is_disjoint; }
    void set_is_disjoint(std::optional<bool> value) { this->is_disjoint = value; }

    /**
     * Identifies the model element the link connects.
     */
    std::optional<std::wstring> get_element() const { return element; }
    void set_element(std::optional<std::wstring> value) { if (value) CheckConstraint(L"element", element_constraint, *value); this->element = value; }

    /**
     * Identifies the note the link connects.
     */
    std::optional<std::wstring> get_note() const { return note; }
    void set_note(std::optional<std::wstring> value) { if (value) CheckConstraint(L"note", note_constraint, *value); this->note = value; }
};

/**
 * A object that represents a point in a diagram through (x,y) coordinates (horizontal and
 * vertical), where the top left corner of the diagram represents the coordinates (0,0)
 * which increase downwards and rightwards.
 *
 * Determines the coordinates of the top left corner of the rectangular shape using a point
 * object.
 */
class Point {
public:
    Point() :
        x_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt),
        y_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt)
    {}
    virtual ~Point() = default;

private:
    int64_t x;
    ClassMemberConstraints x_constraint;
    int64_t y;
    ClassMemberConstraints y_constraint;

public:
    /**
     * Determines the horizontal coordinate of a point using a positive integer.
     */
    const int64_t& get_x() const { return x; }
    int64_t& get_mutable_x() { return x; }
    void set_x(const int64_t& value) { CheckConstraint(L"x", x_constraint, value); this->x = value; }

    /**
     * Determines the vertical coordinate of a point using a positive integer.
     */
    const int64_t& get_y() const { return y; }
    int64_t& get_mutable_y() { return y; }
    void set_y(const int64_t& value) { CheckConstraint(L"y", y_constraint, value); this->y = value; }
};

enum class OntoUmlElementType : int { BINARY_RELATION, BINARY_RELATION_VIEW, CLASS, CLASS_VIEW, DIAMOND, GENERALIZATION, GENERALIZATION_SET, GENERALIZATION_SET_VIEW, GENERALIZATION_VIEW, LINK, LINK_VIEW, LITERAL, NARY_RELATION, NARY_RELATION_VIEW, NOTE, NOTE_VIEW, PACKAGE, PACKAGE_VIEW, PATH, PROJECT, PROPERTY, RECTANGLE, TEXT };

/**
 * An identified element of an OntoUML ontology according to the OntoUML Metamodel, which
 * includes projects, model elements, diagrams, views, and shapes.
 *
 * An OntoUML element that can be assigned a name and other descriptive information.
 *
 * A named element that serves as the container of an entire OntoUML ontology, including the
 * elements of both the abstract syntax (i.e., model elements) and the concrete syntax
 * (i.e., diagrams, view, and shapes).
 *
 * A named element that represents an element of the language's abstract syntax (e.g., a
 * class, a relation, or a generalization).
 *
 * A model element that represents the generalization of a specific classifier into a
 * general classifier. When read in the inverse direction, a generalization is referred to
 * as a specialization.
 *
 * Examples include the generalization of a specific class "Student" into a general class
 * "Person," and the generalization of a specific relation "close friends with" into a
 * general relation "friends with".
 *
 * A generalization can only connect two classifiers of the same type, i.e., it can either
 * connect two class elements or two relation elements.
 *
 * A model element that represents a group of connected generalization elements. A
 * generalization set can define disjoint and/or complete constraints over the
 * generalizations it groups.
 *
 * Examples include the incomplete (i.e., non-complete) and overlapping (i.e., non-disjoint)
 * generalization set of "Person" into "Student" and "Teacher", and the disjoint and
 * complete generalization set of "Person" into "Child" and "Adult".
 *
 * All generalizations in the generalization set must share a common general classifier.
 *
 * A model element that connects a note to a model element it concerns.
 *
 * A model element that represents a specific value within an enumerated set of values.
 * Examples include each letter in an A to F letter grading scale, listed in a class "Letter
 * Grade" decorated with the stereotype "enumeration".
 *
 * A model element that contains an annotation about the ontology or some of its elements. A
 * note can also be used to represent a constraint in both natural or structured language
 * (i.e., first-order logic, or OCL).
 *
 * A model element that can group other model elements that are referred to as "packageable
 * elements." Package elements are used to perform the modularization of an ontology.
 *
 * While the OntoUML Metamodel does not require package elements to follow a tree structure
 * (i.e., it allows overlapping packages), ontologies that require UML representations
 * should adhere to this constraint for compatibility.
 *
 * A model element that can be decorated with a stereotype to identify its ontological
 * properties according to the Unified Foundational Ontology (UFO).
 *
 * Examples include a class decorated with the stereotype "kind" identifying it as a type of
 * object that provides an identity principle to its instances.
 *
 * A decoratable element that represents an attribute of a class, or one end of a relation.
 *
 * Examples include the attribute "name" of the class "Person", and the ends of the binary
 * relation "studies in" connected to the classes "Student" and "University."
 *
 * Instances of class and relation elements bear values for the properties these classifiers
 * contain, according to the constraints specified within each property. For example, the
 * value assigned to a property in an instance must be itself an instance of the classifier
 * in property type.
 *
 * A decoratable element (either a class or a relation) that defines properties exhibited by
 * its instances.
 *
 * A classifier that defines the properties of a set of "individualized" entities (i.e.,
 * non-relational) of the subject domain.
 *
 * Examples include "Person", "Enrollment", and "Grade".
 *
 * The instances of a class may include entities such as objects (e.g., people,
 * organizations, vehicles), reified properties (e.g., leafs' colors, agents' intentions,
 * enrollments), and bare values (e.g., a number or a literal).
 *
 * A relation that defines the properties of a set of binary relations of the subject
 * domain.
 *
 * Examples include "studies in", and derivation relations (e.g., between material relations
 * and relators).
 *
 * A binary relation may either connect two classes, or a relation (as source) and a class
 * (as target) in the case of derivation relations connecting descriptive relations to the
 * classes that serve as their truthmakers (as in the relation between the material relation
 * "studies in" and the "Enrollment" relator).
 *
 * A relation that defines the properties of a set of relations of the subject domain that
 * connect more than two members.
 *
 * Examples include "studies in", "buys product from" (ternary relation), and derivation
 * relations (e.g., between material relations and relators).
 *
 * An OntoUML element that identifies how to render a view (or a portion of one) in a
 * diagram.
 *
 * A shape defined by a list of points connecting two other shapes.
 *
 * A shape defined by a top left position, a height, a width.
 *
 * A rectangular shape that renders the joining diamond of a n-ary relation view.
 *
 * A rectangular shape that renders the shape of a class view or a package view.
 *
 * A rectangular shape that renders the shape of a generalization set view or a note view.
 *
 * An OntoUML element that represents a single model element in a diagram.
 *
 * A view element connects a model element to the shapes in a diagram necessary to represent
 * a single occurrence of it. For example, an n-ary relation view connects a single relation
 * element to one diamond and a set of paths that represent a single occurrence of it in a
 * diagram. Multiple views can represent multiple occurrences of an element in the same
 * diagram.
 *
 * A view element is responsible for what portions of a model element are present in a
 * single diagram representation (e.g., whether the cardinality of a property is shown),
 * unlike a shape, which is responsible for aspects of the actual drawing (e.g., how to
 * render a portion of a view, in which position, and with which dimensions).
 *
 * A view element that represents the single occurrence of a class in a diagram.
 *
 * A view element that represents the single occurrence of a generalization set in a
 * diagram.
 *
 * A view element that represents the single occurrence of a n-ary relation in a diagram.
 *
 * A view element that represents the single occurrence of a note in a diagram.
 *
 * A view element that represents the single occurrence of a package in a diagram.
 *
 * A view element that represents the single occurrence of a binary connector (e.g., a
 * binary relation, or a generalization) in a diagram.
 *
 * A view element that represents the single occurrence of a binary relation in a diagram.
 *
 * A view element that represents the single occurrence of a link in a diagram.
 *
 * The OntoUML Schema defines the JSON serializations of OntoUML ontologies and their
 * contents according to the OntoUML Metamodel project.
 */
class Coordinate {
public:
    Coordinate() :
        id_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        general_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        specific_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        element_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        note_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        height_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt),
        width_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt),
        is_view_of_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        rectangle_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        diamond_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        path_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        source_view_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt),
        target_view_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, 1, std::nullopt, std::nullopt)
    {}
    virtual ~Coordinate() = default;

private:
    std::wstring created;
    std::wstring id;
    ClassMemberConstraints id_constraint;
    std::optional<std::wstring> modified;
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> alternative_names;
    std::optional<std::vector<Resource>> contributors;
    std::optional<std::vector<Resource>> creators;
    std::optional<std::map<std::wstring, std::wstring>> description;
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> editorial_notes;
    std::optional<std::map<std::wstring, std::wstring>> name;
    std::optional<std::vector<Resource>> access_rights;
    std::optional<std::vector<std::wstring>> acronyms;
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> bibliographic_citations;
    std::optional<std::vector<Resource>> contexts;
    std::optional<std::vector<Resource>> designed_for_tasks;
    std::optional<std::vector<Element>> elements;
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> keywords;
    std::optional<std::vector<std::wstring>> landing_pages;
    std::optional<std::vector<std::wstring>> languages;
    std::optional<Resource> license;
    std::optional<std::wstring> onto_uml_element_namespace;
    std::optional<std::vector<Resource>> ontology_types;
    std::optional<Resource> publisher;
    std::optional<Resource> representation_style;
    std::optional<std::wstring> root;
    std::optional<std::vector<std::wstring>> sources;
    std::optional<std::vector<Resource>> themes;
    OntoUmlElementType type;
    std::optional<std::map<std::wstring, nlohmann::json>> custom_properties;
    std::optional<std::wstring> general;
    ClassMemberConstraints general_constraint;
    std::optional<std::wstring> specific;
    ClassMemberConstraints specific_constraint;
    std::optional<std::wstring> categorizer;
    std::optional<std::vector<std::wstring>> generalizations;
    std::optional<bool> is_complete;
    std::optional<bool> is_disjoint;
    std::optional<std::wstring> element;
    ClassMemberConstraints element_constraint;
    std::optional<std::wstring> note;
    ClassMemberConstraints note_constraint;
    std::optional<Text> text;
    std::optional<std::vector<std::wstring>> contents;
    std::optional<bool> is_derived;
    std::optional<std::wstring> stereotype;
    std::optional<AggregationKind> aggregation_kind;
    std::optional<std::wstring> cardinality;
    std::optional<bool> is_ordered;
    std::optional<bool> is_read_only;
    std::optional<std::wstring> property_type;
    std::optional<std::vector<std::wstring>> redefined_properties;
    std::optional<std::vector<std::wstring>> subsetted_properties;
    std::optional<bool> is_abstract;
    std::optional<std::vector<std::wstring>> properties;
    std::optional<bool> is_powertype;
    std::optional<std::vector<std::wstring>> literals;
    std::optional<std::wstring> order;
    std::optional<std::vector<RestrictedTo>> restricted_to;
    std::optional<std::vector<Point>> points;
    std::optional<int64_t> height;
    ClassMemberConstraints height_constraint;
    std::optional<Point> top_left;
    std::optional<int64_t> width;
    ClassMemberConstraints width_constraint;
    std::optional<std::wstring> is_view_of;
    ClassMemberConstraints is_view_of_constraint;
    std::optional<std::wstring> rectangle;
    ClassMemberConstraints rectangle_constraint;
    std::optional<std::wstring> diamond;
    ClassMemberConstraints diamond_constraint;
    std::optional<std::vector<std::wstring>> members;
    std::optional<std::vector<std::wstring>> paths;
    std::optional<std::wstring> path;
    ClassMemberConstraints path_constraint;
    std::optional<std::wstring> source_view;
    ClassMemberConstraints source_view_constraint;
    std::optional<std::wstring> target_view;
    ClassMemberConstraints target_view_constraint;

public:
    /**
     * Determines when the element was created using a string in one of the following formats:
     * year, year-month, date, or date-time.
     */
    const std::wstring& get_created() const { return created; }
    std::wstring& get_mutable_created() { return created; }
    void set_created(const std::wstring& value) { this->created = value; }

    /**
     * Determines the unique identifier for an OntoUML element in an ontology using a non-empty
     * string.
     */
    const std::wstring& get_id() const { return id; }
    std::wstring& get_mutable_id() { return id; }
    void set_id(const std::wstring& value) { CheckConstraint(L"id", id_constraint, value); this->id = value; }

    /**
     * Determines when the element was modified using a string in one of the following formats:
     * year, year-month, date, or date-time.
     */
    std::optional<std::wstring> get_modified() const { return modified; }
    void set_modified(std::optional<std::wstring> value) { this->modified = value; }

    /**
     * Determines alternative names of the named element using an array of language strings.
     * Alternative names are not translations of the named element, but indeed alternatives or
     * synonyms to the main one.
     */
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> get_alternative_names() const { return alternative_names; }
    void set_alternative_names(std::optional<std::vector<std::map<std::wstring, std::wstring>>> value) { this->alternative_names = value; }

    /**
     * Identifies the agents who contributed to the development of the named element.
     */
    std::optional<std::vector<Resource>> get_contributors() const { return contributors; }
    void set_contributors(std::optional<std::vector<Resource>> value) { this->contributors = value; }

    /**
     * Identifies the agents who contributed to the creation of the named element.
     */
    std::optional<std::vector<Resource>> get_creators() const { return creators; }
    void set_creators(std::optional<std::vector<Resource>> value) { this->creators = value; }

    /**
     * Determines a free-text account of the named element using language string.
     */
    std::optional<std::map<std::wstring, std::wstring>> get_description() const { return description; }
    void set_description(std::optional<std::map<std::wstring, std::wstring>> value) { this->description = value; }

    /**
     * Determines general notes about the named element using an array of language strings.
     * Editorial notes are typically notes on the development process and must not be confused
     * with descriptions.
     */
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> get_editorial_notes() const { return editorial_notes; }
    void set_editorial_notes(std::optional<std::vector<std::map<std::wstring, std::wstring>>> value) { this->editorial_notes = value; }

    /**
     * Determines the name of the named element using language string.
     */
    std::optional<std::map<std::wstring, std::wstring>> get_name() const { return name; }
    void set_name(std::optional<std::map<std::wstring, std::wstring>> value) { this->name = value; }

    /**
     * Identifies a document or a text concerning who and how the project can be accessed. E.g.,
     * the document <http://publications.europa.eu/resource/authority/access-right/PUBLIC>
     * informs that something is "publicly accessible by everyone."
     */
    std::optional<std::vector<Resource>> get_access_rights() const { return access_rights; }
    void set_access_rights(std::optional<std::vector<Resource>> value) { this->access_rights = value; }

    /**
     * Determines the acronyms one can use to refer to the project using strings. E.g.,
     * "RDBS-O", "COVER", "ROT".
     */
    std::optional<std::vector<std::wstring>> get_acronyms() const { return acronyms; }
    void set_acronyms(std::optional<std::vector<std::wstring>> value) { this->acronyms = value; }

    /**
     * Determines bibliographic references for the project using language strings. E.g.,
     * "Weigand, H., Johannesson, P., & Andersson, B. (2021). An artifact ontology for design
     * science research. Data & Knowledge Engineering, 133".
     */
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> get_bibliographic_citations() const { return bibliographic_citations; }
    void set_bibliographic_citations(std::optional<std::vector<std::map<std::wstring, std::wstring>>> value) { this->bibliographic_citations = value; }

    /**
     * Identifies the contexts in which the project was developed. The OntoUML/UFO Catalog
     * Metadata Vocabulary (OCMV) provides a set of recurrent modeling contexts:
     *
     * - ocmv:Research: The project was developed as part of a research project. This usually
     * implies that the project was featured in a scientific publication.
     *
     * - ocmv:Industry: The project was developed for a public or private organization.
     *
     * - ocmv:Classroom: The project was developed within the context of a course on conceptual
     * modeling, most likely as a course assignment.
     */
    std::optional<std::vector<Resource>> get_contexts() const { return contexts; }
    void set_contexts(std::optional<std::vector<Resource>> value) { this->contexts = value; }

    /**
     * Identifies goals that motivated the development of the project. The OntoUML/UFO Catalog
     * Metadata Vocabulary (OCMV) provides a set of goals that be reused as recurrent modeling
     * goals:
     *
     * - ocmv:ConceptualClarification: The project was created as the result of an ontological
     * analysis of a concept, language, or domain of interest that sought to conceptually
     * clarify and untangle complex notions and relations.
     *
     * - ocmv:DataPublication: The project was created to support the publication of some
     * datasets. For instance, in the case of a conceptual model used to generate an OWL
     * vocabulary to publish tabular data as linked open data on the web.
     *
     * - ocmv:DecisionSupportSystem: The project was created during the development of a
     * decision support system.
     *
     * - ocmv:Example: The project was created to demonstrate how OntoUML can be used to solve a
     * certain modeling challenge, to support an experiment involving OntoUML, or to exemplify
     * how a generic model can be reused in more concrete scenarios.
     *
     * - ocmv:InformationRetrieval: The project was created to support the design of an
     * information retrieval system.
     *
     * - ocmv:Interoperability: The project was created to support data integration, vocabulary
     * alignment, or the interoperability of software systems.
     *
     * - ocmv:LanguageEngineering: The project was created for the design of a domain-specific
     * modeling language.
     *
     * - ocmv:Learning: The project was created so that its authors could learn UFO and OntoUML.
     * This usually applies to models developed by students as part of their course
     * assignments.
     *
     * - ocmv:SoftwareEngineering: The project was created during the development of an
     * information system. For instance, in the case of a conceptual model used to generate a
     * relational database.
     */
    std::optional<std::vector<Resource>> get_designed_for_tasks() const { return designed_for_tasks; }
    void set_designed_for_tasks(std::optional<std::vector<Resource>> value) { this->designed_for_tasks = value; }

    /**
     * Contains the OntoUML elements that are part of the project.
     */
    std::optional<std::vector<Element>> get_elements() const { return elements; }
    void set_elements(std::optional<std::vector<Element>> value) { this->elements = value; }

    /**
     * Determines the domains (partially) described by the project using language strings. E.g.,
     * the User Feedback Ontology is described with the keywords "online user feedback",
     * "software engineering", and "requirements engineering".
     */
    std::optional<std::vector<std::map<std::wstring, std::wstring>>> get_keywords() const { return keywords; }
    void set_keywords(std::optional<std::vector<std::map<std::wstring, std::wstring>>> value) { this->keywords = value; }

    /**
     * Identifies the web pages where one can access the project. E.g.,
     * <https://www.model-a-platform.com> is the landing page of the Digital Platform Ontology.
     */
    std::optional<std::vector<std::wstring>> get_landing_pages() const { return landing_pages; }
    void set_landing_pages(std::optional<std::vector<std::wstring>> value) { this->landing_pages = value; }

    /**
     * Determines the languages in which the lexical labels of the project are written using
     * strings. This field should use values listed in the IANA Language Sub Tag Registry, such
     * as, "en" and "pt".
     */
    std::optional<std::vector<std::wstring>> get_languages() const { return languages; }
    void set_languages(std::optional<std::vector<std::wstring>> value) { this->languages = value; }

    /**
     * Identifies a legal document under which the project is made available, e.g., "CC BY 4.0"
     * <https://creativecommons.org/licenses/by/4.0/>.
     */
    std::optional<Resource> get_license() const { return license; }
    void set_license(std::optional<Resource> value) { this->license = value; }

    /**
     * Determines the namespace for all OntoUML elements in the project using a string in URI
     * format. When present, the namespace appended with the id of any element in the project
     * should form a globally unique resolvable URI for identification in the web as a FAIR
     * Digital Object.
     */
    std::optional<std::wstring> get_onto_uml_element_namespace() const { return onto_uml_element_namespace; }
    void set_onto_uml_element_namespace(std::optional<std::wstring> value) { this->onto_uml_element_namespace = value; }

    /**
     * Identifies the classifications of the project according to its scope. The OntoUML/UFO
     * Catalog Metadata Vocabulary (OCMV) provides a set of recurrent modeling scopes based on
     * the work of Roussey et al. (2011):
     *
     * - ocmv:Core: An ontology that grasps the central concepts and relations of a given
     * domain, possibly integrating several domain ontologies and being applicable in multiple
     * scenarios. E.g., UFO-S, a commitment-based ontology of services, can be considered a core
     * ontology because it applies to services in multiple domains, such as medical, financial,
     * and legal services.
     *
     * - ocmv:Domain: An ontology that describes how a community conceptualizes a phenomenon of
     * interest. In general, a domain ontology formally characterizes a much narrower domain
     * than a core ontology does. E.g., OntoBio is a domain ontology of biodiversity.
     *
     * - ocmv:Application: An ontology that specializes a domain ontology where there could be
     * no consensus or knowledge sharing. It represents the particular model of a domain
     * according to a single viewpoint of a user or a developer.
     */
    std::optional<std::vector<Resource>> get_ontology_types() const { return ontology_types; }
    void set_ontology_types(std::optional<std::vector<Resource>> value) { this->ontology_types = value; }

    /**
     * Identifies the agent responsible for making the project available. The publisher does not
     * need to have created or contributed to the resource.
     */
    std::optional<Resource> get_publisher() const { return publisher; }
    void set_publisher(std::optional<Resource> value) { this->publisher = value; }

    /**
     * Identifies the representation style adopted in the project between "OntoUML Style" and
     * "UFO Style":
     *
     * - ocmv:OntoumlStyle: Characterizes a project that contains at least one class, relation,
     * or property using a valid OntoUML stereotype.
     *
     * - ocmv:UfoStyle: Characterizes a project that contains at least one class or relation
     * from UFO without an OntoUML stereotype.
     */
    std::optional<Resource> get_representation_style() const { return representation_style; }
    void set_representation_style(std::optional<Resource> value) { this->representation_style = value; }

    /**
     * Identifies the root package of a project (the package containing all other model elements
     * of the project) if present.
     */
    std::optional<std::wstring> get_root() const { return root; }
    void set_root(std::optional<std::wstring> value) { this->root = value; }

    /**
     * Identifies resources that contain, present, or significantly influenced the development
     * of the project. We recommend the use of persistent and resolvable identifiers to refer to
     * these resources, such as the Digital Object Identifier (DOI) or DBLP''s URI, such as,
     * <https://doi.org/10.3233/AO-150150>, or
     * <https://dblp.org/rec/journals/ao/Morales-Ramirez15>.
     */
    std::optional<std::vector<std::wstring>> get_sources() const { return sources; }
    void set_sources(std::optional<std::vector<std::wstring>> value) { this->sources = value; }

    /**
     * Identifies the central themes of the project according to a theme taxonomy. In the
     * OntoUML/UFO Catalog Metadata Vocabulary (OCMV), for example, the theme of a project must
     * be a concept from the Library of Congress Classification (LCC), such as, "Class S -
     * Agriculture" or "Class T - Technology.""
     */
    std::optional<std::vector<Resource>> get_themes() const { return themes; }
    void set_themes(std::optional<std::vector<Resource>> value) { this->themes = value; }

    /**
     * Determines an object of type 'Project.'
     *
     * Determines the type of a generalization object.
     *
     * Determines the type of a generalization set object.
     *
     * Determines the type of a link object.
     *
     * Determines the type of a literal object.
     *
     * Determines the type of a note object.
     *
     * Determines the type of a package object.
     *
     * Determines the type of a property object.
     *
     * Determines the type of a class object.
     *
     * Determines the type of a binary relation object.
     *
     * Determines the type of a n-ary relation object.
     *
     * Determines the type of a path object.
     *
     * Determines the type of a diamond object.
     *
     * Determines the type of a rectangle object.
     *
     * Determines the type of a text object.
     *
     * Determines the type of a class view object.
     *
     * Determines the type of a generalization set view object.
     *
     * Determines the type of a n-ary relation view object.
     *
     * Determines the type of a note view object.
     *
     * Determines the type of a package view object.
     *
     * Determines the type of a generalization view object.
     *
     * Determines the type of a link view object.
     *
     * Determines the type of a binary relation view object.
     */
    const OntoUmlElementType& get_type() const { return type; }
    OntoUmlElementType& get_mutable_type() { return type; }
    void set_type(const OntoUmlElementType& value) { this->type = value; }

    /**
     * Determines custom properties of the model element using key-value pairs. In UML, for
     * instance, this custom properties are represented through tagged values.
     */
    std::optional<std::map<std::wstring, nlohmann::json>> get_custom_properties() const { return custom_properties; }
    void set_custom_properties(std::optional<std::map<std::wstring, nlohmann::json>> value) { this->custom_properties = value; }

    /**
     * Identifies the general classifier in a generalization element. E.g., in the
     * generalization of "Student" into "Person", "Person" is the general classifier.
     */
    std::optional<std::wstring> get_general() const { return general; }
    void set_general(std::optional<std::wstring> value) { if (value) CheckConstraint(L"general", general_constraint, *value); this->general = value; }

    /**
     * Identifies the general classifier in a generalization element. E.g., in the
     * generalization of "Student" into "Person", "Student" is the specific classifier.
     */
    std::optional<std::wstring> get_specific() const { return specific; }
    void set_specific(std::optional<std::wstring> value) { if (value) CheckConstraint(L"specific", specific_constraint, *value); this->specific = value; }

    /**
     * Identifies the high-order class that classifies (i.e., is instantiated by) every specific
     * class in the generalization set.
     *
     * For example, "Academic Role" as the categorizer of the generalization set of "Person"
     * into "Student" and "Teacher" representing the specific classes as instances of the
     * categorizer.
     *
     * A categorizer can only be present in generalization sets involving exclusively classes.
     */
    std::optional<std::wstring> get_categorizer() const { return categorizer; }
    void set_categorizer(std::optional<std::wstring> value) { this->categorizer = value; }

    /**
     * Identifies all generalizations that are involved by the generalization set.
     *
     * Identifies the generalization views that are grouped by the generalization set view in
     * the diagram.
     */
    std::optional<std::vector<std::wstring>> get_generalizations() const { return generalizations; }
    void set_generalizations(std::optional<std::vector<std::wstring>> value) { this->generalizations = value; }

    /**
     * Determines whether the specific classifiers in the generalization set completely cover
     * the extension of the general classifier.
     *
     * Examples include the generalization set involving "Child" and "Adult" generalized into
     * "Person", where the "is complete" as "true" indicates that every person is either an
     * instance of "Child" or "Adult."
     */
    std::optional<bool> get_is_complete() const { return is_complete; }
    void set_is_complete(std::optional<bool> value) { this->is_complete = value; }

    /**
     * Determines whether the specific classifiers in the generalization set have disjoint
     * extensions.
     *
     * Examples include the generalization set involving "Child" and "Adult" generalized into
     * "Person", where the "is disjoint" as "true" indicates that no person is simultaneously an
     * instance of "Child" and "Adult."
     */
    std::optional<bool> get_is_disjoint() const { return is_disjoint; }
    void set_is_disjoint(std::optional<bool> value) { this->is_disjoint = value; }

    /**
     * Identifies the model element the link connects.
     */
    std::optional<std::wstring> get_element() const { return element; }
    void set_element(std::optional<std::wstring> value) { if (value) CheckConstraint(L"element", element_constraint, *value); this->element = value; }

    /**
     * Identifies the note the link connects.
     */
    std::optional<std::wstring> get_note() const { return note; }
    void set_note(std::optional<std::wstring> value) { if (value) CheckConstraint(L"note", note_constraint, *value); this->note = value; }

    /**
     * Determines the contents of a note using a language string.
     *
     * Identifies the text shape that renders the generalization set view in the diagram.
     *
     * Identifies the text shape that renders the note view in the diagram.
     */
    std::optional<Text> get_text() const { return text; }
    void set_text(std::optional<Text> value) { this->text = value; }

    /**
     * Identifies the contents of a package element.
     */
    std::optional<std::vector<std::wstring>> get_contents() const { return contents; }
    void set_contents(std::optional<std::vector<std::wstring>> value) { this->contents = value; }

    /**
     * Determines whether the model element is derived from a different one, i.e., whether a
     * different element serves as its truthmaker.
     *
     * Examples include the comparative relation "is heavier than" between two physical objects
     * which is derived from their "Weight" quality.
     */
    std::optional<bool> get_is_derived() const { return is_derived; }
    void set_is_derived(std::optional<bool> value) { this->is_derived = value; }

    /**
     * Determines the type of a model element according to Unified Foundational Ontology (UFO).
     */
    std::optional<std::wstring> get_stereotype() const { return stereotype; }
    void set_stereotype(std::optional<std::wstring> value) { this->stereotype = value; }

    /**
     * Determines whether the property (a relation end) is a whole in a parthood (mereological)
     * relation using an enumerated string.
     *
     * Examples include "Project Member" as a "shared" part of a "Project Team" and an "Engine"
     * as a "composite" part of a "Vehicle."
     *
     * The possible values for aggregation kind are "composite" (when parts are exclusive to one
     * whole), "shared" (when parts can be shared), and "none" for non-parthood relations. The
     * "null" is also interpreted as "none."
     */
    std::optional<AggregationKind> get_aggregation_kind() const { return aggregation_kind; }
    void set_aggregation_kind(std::optional<AggregationKind> value) { this->aggregation_kind = value; }

    /**
     * Determines the cardinality of a property using a string.
     *
     * Examples include "Person" with cardinality "1" (exactly one) in the property (attribute)
     * "name", and cardinality "0..*" (zero or more) in the property (related end) "friends" of
     * the relation "friends with".
     *
     * The cardinality should follow the regular expression "^\d+(\.\.(\d+|\*))?$", where the
     * number before ".." represents the lower bound, and the number or "*" after represents the
     * upper bound. In case the ".." is not present, a number represents an exact number of
     * instances, and "*" represents "zero or more" instances.
     *
     * This regular expression is not enforced to accommodate theoretical ranges as expression,
     * such as, "a..b".
     */
    std::optional<std::wstring> get_cardinality() const { return cardinality; }
    void set_cardinality(std::optional<std::wstring> value) { this->cardinality = value; }

    /**
     * Determines whether the order of assignments of a property carries meaning using a
     * boolean. This boolean only regards properties whose maximum cardinality is greater than
     * one.
     */
    std::optional<bool> get_is_ordered() const { return is_ordered; }
    void set_is_ordered(std::optional<bool> value) { this->is_ordered = value; }

    /**
     * Determines whether the assignments of a property are immutable using a boolean.
     */
    std::optional<bool> get_is_read_only() const { return is_read_only; }
    void set_is_read_only(std::optional<bool> value) { this->is_read_only = value; }

    /**
     * Identifies the classifier instantiated by the values assigned to the property.
     */
    std::optional<std::wstring> get_property_type() const { return property_type; }
    void set_property_type(std::optional<std::wstring> value) { this->property_type = value; }

    /**
     * Identifies the properties (relation ends) that provide redefinition constraints to the
     * property.
     *
     * Examples include, in the relations (i) "works in" from "Project Member" to "Project", and
     * (ii) "leads" from "Project Leader" to "Project", where the member end is redefined for
     * the leader who can only be connected to the project through that specific relation.
     *
     * Redefinition can only occur on the relation ends when both sides of the involved
     * relations are connected to the same classifiers or classifiers that are in the same
     * generalization chain.
     */
    std::optional<std::vector<std::wstring>> get_redefined_properties() const { return redefined_properties; }
    void set_redefined_properties(std::optional<std::vector<std::wstring>> value) { this->redefined_properties = value; }

    /**
     * Identifies the properties (relation ends) that provide subsetting constraints to the
     * property.
     *
     * Examples include, in the relations (i) "works for" from "Employee" to "Organization", and
     * (ii) "teaches for" from "Teacher" to "University", both relation ends of (ii) are subsets
     * of their respect ends in (i).
     *
     * Subsetting can only occur on the relation ends when both sides of the involved relations
     * are connected to the same classifiers or classifiers that are in the same generalization
     * chain.
     *
     * Subsetting can also be represented through a generalization between relations.
     */
    std::optional<std::vector<std::wstring>> get_subsetted_properties() const { return subsetted_properties; }
    void set_subsetted_properties(std::optional<std::vector<std::wstring>> value) { this->subsetted_properties = value; }

    /**
     * Determines whether the classifier can have direct instances using a boolean. Abstract
     * classifiers can only have instances when these are instances of some other classifier
     * that is not abstract (i.e., concrete) and is a specialization of the abstract one.
     */
    std::optional<bool> get_is_abstract() const { return is_abstract; }
    void set_is_abstract(std::optional<bool> value) { this->is_abstract = value; }

    /**
     * Identifies the properties contained in a classifier. These properties are referred to as
     * attributes when contained by classes, and relation ends when contained by relations. In
     * the case of relations, the properties array must be ordered.
     */
    std::optional<std::vector<std::wstring>> get_properties() const { return properties; }
    void set_properties(std::optional<std::vector<std::wstring>> value) { this->properties = value; }

    /**
     * Determines whether the high-order class is a "Cardelli powertype" using a boolean. In
     * other words, determines whether the high-order class is defined as the one whose
     * instances are its base type plus all possible specializations of it.
     */
    std::optional<bool> get_is_powertype() const { return is_powertype; }
    void set_is_powertype(std::optional<bool> value) { this->is_powertype = value; }

    /**
     * Identifies the literals of an enumeration class.
     */
    std::optional<std::vector<std::wstring>> get_literals() const { return literals; }
    void set_literals(std::optional<std::vector<std::wstring>> value) { this->literals = value; }

    /**
     * Determines the instantiation order of a class using a string. Examples include ordered
     * classes such as first-order classes (order "1"), second-order classes (order "2"), and
     * third-order classes (order "3"), as well as orderless classes (order "*").
     */
    std::optional<std::wstring> get_order() const { return order; }
    void set_order(std::optional<std::wstring> value) { this->order = value; }

    /**
     * Determines the possible ontological natures of the instances of a class using an array of
     * enumerated strings.
     *
     * Examples include the class "Vehicle" restricted to having "functional-complex" instances
     * and the class "Insured Item" restricted to "functional-complex" and "relator" (e.g.,
     * employment insurance).
     */
    std::optional<std::vector<RestrictedTo>> get_restricted_to() const { return restricted_to; }
    void set_restricted_to(std::optional<std::vector<RestrictedTo>> value) { this->restricted_to = value; }

    /**
     * Determines the points across the path shape is rendered using an array of point objects.
     * A path has a minimum of two points which must be ordered from the edge of the source
     * shape to the edge of the target shape.
     */
    std::optional<std::vector<Point>> get_points() const { return points; }
    void set_points(std::optional<std::vector<Point>> value) { this->points = value; }

    /**
     * Determines the height of the rectangular shape using a positive integer.
     */
    std::optional<int64_t> get_height() const { return height; }
    void set_height(std::optional<int64_t> value) { if (value) CheckConstraint(L"height", height_constraint, *value); this->height = value; }

    /**
     * Determines the coordinates of the top left corner of the rectangular shape using a point
     * object.
     */
    std::optional<Point> get_top_left() const { return top_left; }
    void set_top_left(std::optional<Point> value) { this->top_left = value; }

    /**
     * Determines the width of the rectangular shape using a positive integer.
     */
    std::optional<int64_t> get_width() const { return width; }
    void set_width(std::optional<int64_t> value) { if (value) CheckConstraint(L"width", width_constraint, *value); this->width = value; }

    /**
     * Identifies the model element that the view represents in the diagram.
     */
    std::optional<std::wstring> get_is_view_of() const { return is_view_of; }
    void set_is_view_of(std::optional<std::wstring> value) { if (value) CheckConstraint(L"is_view_of", is_view_of_constraint, *value); this->is_view_of = value; }

    /**
     * Identifies the rectangle shape that renders the class view in the diagram.
     *
     * Identifies the rectangle shape that renders the package view in the diagram.
     */
    std::optional<std::wstring> get_rectangle() const { return rectangle; }
    void set_rectangle(std::optional<std::wstring> value) { if (value) CheckConstraint(L"rectangle", rectangle_constraint, *value); this->rectangle = value; }

    /**
     * Identifies the diamond shape that renders the joining of all paths of the n-ary relation
     * in the diagram.
     */
    std::optional<std::wstring> get_diamond() const { return diamond; }
    void set_diamond(std::optional<std::wstring> value) { if (value) CheckConstraint(L"diamond", diamond_constraint, *value); this->diamond = value; }

    /**
     * Identifies the class views (i.e., the members) that are connected by the n-ary relation
     * view in the diagram. This array of member views must be ordered according to the
     * properties of the relation the view represents.
     */
    std::optional<std::vector<std::wstring>> get_members() const { return members; }
    void set_members(std::optional<std::vector<std::wstring>> value) { this->members = value; }

    /**
     * Identifies the path shapes that render each path of the n-ary relation view in the
     * diagram. This array of paths must be ordered according to the properties of the relation
     * the view represents.
     */
    std::optional<std::vector<std::wstring>> get_paths() const { return paths; }
    void set_paths(std::optional<std::vector<std::wstring>> value) { this->paths = value; }

    /**
     * Identifies the path shape that renders the binary connector in the diagram.
     */
    std::optional<std::wstring> get_path() const { return path; }
    void set_path(std::optional<std::wstring> value) { if (value) CheckConstraint(L"path", path_constraint, *value); this->path = value; }

    /**
     * Identifies the source view the binary connector view connects in the diagram.
     */
    std::optional<std::wstring> get_source_view() const { return source_view; }
    void set_source_view(std::optional<std::wstring> value) { if (value) CheckConstraint(L"source_view", source_view_constraint, *value); this->source_view = value; }

    /**
     * Identifies the target view the binary connector view connects in the diagram.
     */
    std::optional<std::wstring> get_target_view() const { return target_view; }
    void set_target_view(std::optional<std::wstring> value) { if (value) CheckConstraint(L"target_view", target_view_constraint, *value); this->target_view = value; }
};
