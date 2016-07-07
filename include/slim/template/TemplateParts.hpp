#pragma once
#include "Template.hpp"
#include "../expression/Expression.hpp"
namespace slim
{
    class Symbol;

    /**@brief List of parts within a single block for sequential evaluation. */
    class TemplatePartsList : public TemplatePart
    {
    public:
        TemplatePartsList(std::vector<std::unique_ptr<TemplatePart>> &&parts)
            : parts(std::move(parts))
        {}
        virtual std::string to_string()const override
        {
            std::string out;
            for (auto &part : parts) out += part->to_string();
            return out;
        }
        virtual void render(std::string &buffer, expr::Scope &scope)const override
        {
            for (auto &part : parts)
                part->render(buffer, scope);
        }
    private:
        std::vector<std::unique_ptr<TemplatePart>> parts;
    };
    
    /**@brief Literal text to output directly.*/
    class TemplateText : public TemplatePart
    {
    public:
        TemplateText(std::string &&text) : text(std::move(text)) {}
        virtual std::string to_string()const override { return text; }
        virtual void render(std::string &buffer, expr::Scope &scope)const override
        {
            buffer += text;
        }
    private:
        std::string text;
    };
    /**@brief A script expression to evaulate and write the result. */
    class TemplateOutputExpr : public TemplatePart
    {
    public:
        TemplateOutputExpr(std::unique_ptr<Expression> &&expression);
        ~TemplateOutputExpr();

        virtual std::string to_string()const override;
        virtual void render(std::string &buffer, expr::Scope &scope)const override;
    protected:
        std::unique_ptr<Expression> expression;
    };
    /**@brief A script for loop, containing a template body.*/
    class TemplateForExpr : public TemplatePart
    {
    public:
        TemplateForExpr(
            std::unique_ptr<Expression> &&expr,
            std::unique_ptr<TemplatePart> &&body,
            std::vector<std::shared_ptr<Symbol>> &&param_names);
        ~TemplateForExpr();

        virtual std::string to_string()const override;
        virtual void render(std::string &buffer, expr::Scope &scope)const override;
    protected:
        std::unique_ptr<Expression> expr;
        std::unique_ptr<TemplatePart> body;
        std::vector<std::shared_ptr<Symbol>> param_names;
    };
    struct TemplateCondExpr
    {
        std::unique_ptr<Expression> expr;
        std::unique_ptr<TemplatePart> body;

        TemplateCondExpr(TemplateCondExpr &&) = default;
        TemplateCondExpr& operator =(TemplateCondExpr &&) = default;
    };
    /**@brief A script if, elsif, else sequence with a template body.*/
    class TemplateIfExpr : public TemplatePart
    {
    public:
        TemplateIfExpr(
            TemplateCondExpr && if_expr,
            std::vector<TemplateCondExpr> &&elseif_exprs,
            std::unique_ptr<TemplatePart> &&else_body);
        ~TemplateIfExpr();

        virtual std::string to_string()const override;
        virtual void render(std::string &buffer, expr::Scope &scope)const override;
    protected:
        TemplateCondExpr if_expr;
        std::vector<TemplateCondExpr> elseif_exprs;
        std::unique_ptr<TemplatePart> else_body;
    };
}